// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "pht.h"
#include "token.h"
#include <algorithm>
#include <bit>
#include <cassert>
#include <cerrno>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace pht {
namespace {
enum class table_size_strategy {
    smallest,
    power_of_2,
};

enum class hash_strategy {
    fnv1a32,
    xx3_64,
    intel_crc32,
    lehmer,
    lehmer_128,
};

struct table_strategy {
    table_size_strategy size_strategy;
    character_selection_mask character_selection;
    hash_strategy hasher;
};

struct keyword_statistics {
    unsigned long min_keyword_size;
    unsigned long max_keyword_size;
    std::vector<character_selection_mask> unique_character_selections;
};

struct perfect_hash_table {
    struct table_entry {
        const char* keyword = nullptr;
    };

    keyword_statistics stats;
    unsigned long hash_basis;
    character_selection_mask character_selection;
    hash_strategy hasher;
    unsigned long table_size;

    std::vector<table_entry> entries;
};

template <class Hasher>
bool try_add_all_entries(perfect_hash_table& table) {
    table.entries.resize(table.table_size);
    for (perfect_hash_table::table_entry& entry : table.entries) {
        entry.keyword = nullptr;
    }

    for (keyword_token kt : keyword_tokens) {
        Hasher hasher(table.hash_basis);
        hash_selected_characters(table.character_selection, hasher, kt.keyword, std::strlen(kt.keyword));
        std::uint32_t index = hasher.hash() % table.table_size;
        bool taken = table.entries[index].keyword != nullptr;
        if (taken) {
            return false;
        }
        table.entries[index].keyword = kt.keyword;
    }

    return true;
}

bool try_add_all_entries(perfect_hash_table& table, hash_strategy hasher) {
    switch (hasher) {
        case hash_strategy::fnv1a32:
            return try_add_all_entries<fnv1a32>(table);
        case hash_strategy::xx3_64:
            return try_add_all_entries<xx3_64_hasher>(table);
        case hash_strategy::intel_crc32:
            return try_add_all_entries<intel_crc32_hasher>(table);
        case hash_strategy::lehmer:
            return try_add_all_entries<lehmer_hasher>(table);
        case hash_strategy::lehmer_128:
            return try_add_all_entries<lehmer_128_hasher>(table);
    }
    __builtin_unreachable();
}

// Returns the number of attempts. If it's >= max_attempts, then building the
// table failed.
int try_build_table(perfect_hash_table& table, int max_attempts) {
    table.hash_basis = 0x811c9dc5; // FNV-1a 32-bit basis.
    int attempts = 0;
    for (;;) {
        attempts += 1;
        if (attempts >= max_attempts) {
            return attempts;
        }
        bool succeeded = try_add_all_entries(table, table.hasher);
        if (succeeded) {
            return attempts;
        }
        table.hash_basis += 1;
    }
}

keyword_statistics make_stats() {
    keyword_statistics stats;
    stats.min_keyword_size = 0xffffffff;
    stats.max_keyword_size = 0;
    for (keyword_token kt : keyword_tokens) {
        unsigned long size = std::strlen(kt.keyword);
        stats.min_keyword_size = std::min(stats.min_keyword_size, size);
        stats.max_keyword_size = std::max(stats.max_keyword_size, size);
    }

    for (character_selection_mask m = 1; m < (1<<5); ++m) {
        std::set<std::string> selections;
        for (keyword_token kt : keyword_tokens) {
            std::string selection;
            std::size_t size = std::strlen(kt.keyword);
            if (m & (1 << 0)) selection += kt.keyword[0];
            if (m & (1 << 1)) selection += kt.keyword[1];
            if (m & (1 << 2)) selection += kt.keyword[size-1];
            if (m & (1 << 3)) selection += kt.keyword[size-2];
            if (m & (1 << 4)) selection += size;
            assert(!selection.empty());
            if (selections.count(selection) > 0) {
                // Selection is not unique. Skip.
                goto next_character_selection;
            }
            selections.insert(selection);
        }
        stats.unique_character_selections.push_back(m);
next_character_selection:;
    }
    return stats;
}

perfect_hash_table make_perfect_hash_table(const keyword_statistics& stats, table_strategy strategy) {
    perfect_hash_table table;
    table.stats = stats;

    table.character_selection = strategy.character_selection;
    table.hasher = strategy.hasher;

    unsigned long max_table_size = std::size(keyword_tokens) * 65536;
    switch (strategy.size_strategy) {
        case table_size_strategy::smallest:
            table.table_size = std::size(keyword_tokens);
            break;
        case table_size_strategy::power_of_2:
            table.table_size = std::bit_ceil(std::size(keyword_tokens));
            break;
    }
    for (;;) {
        if (table.table_size > max_table_size) {
            std::fprintf(
                stderr,
                "can't generate table of size %lu from %zu items\n",
                table.table_size,
                std::size(keyword_tokens));
            std::exit(1);
        }
        int max_attempts_per_size = 50'000;
        int attempts = try_build_table(table, max_attempts_per_size);
        if (attempts < max_attempts_per_size) {
            std::fprintf(
                stderr,
                "took %d attempts to generate table of size %lu from %zu items\n",
                attempts,
                table.table_size,
                std::size(keyword_tokens));
            break;
        }
        switch (strategy.size_strategy) {
            case table_size_strategy::smallest:
                table.table_size += 1;
                break;
            case table_size_strategy::power_of_2:
                table.table_size *= 2;
                break;
        }
    }

    return table;
}

void write_table(const char* file_path, const perfect_hash_table& table) {
    FILE* file = std::fopen(file_path, "wb");
    if (file == nullptr) {
        std::fprintf(stderr, "error: failed to open %s: %s\n", file_path, std::strerror(errno));
        std::exit(1);
    }

    std::fprintf(file, R"(
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace pht {
namespace {
constexpr character_selection_mask character_selection = %uU;
constexpr std::uint32_t hash_basis = %luUL;
constexpr std::uint32_t table_size = %luUL;
constexpr std::size_t min_keyword_size = %lu;
constexpr std::size_t max_keyword_size = %lu;

)", table.character_selection, table.hash_basis, table.table_size, table.stats.min_keyword_size, table.stats.max_keyword_size);

    std::fprintf(file, "%s", R"(
struct table_entry {
    const char keyword[max_keyword_size + 1];
    token_type type;
};

constexpr table_entry table[table_size] = {
)");

    for (const perfect_hash_table::table_entry& entry : table.entries) {
        if (entry.keyword == nullptr) {
            std::fprintf(file, "  {\"\", token_type::identifier},\n");
        } else {
            std::fprintf(file, "  {\"%s\", token_type::kw_%s},\n", entry.keyword, entry.keyword);
        }
    }

    const char* hasher_class;
    switch (table.hasher) {
        case hash_strategy::xx3_64: hasher_class = "xx3_64_hasher"; break;
        case hash_strategy::fnv1a32: hasher_class = "fnv1a32"; break;
        case hash_strategy::intel_crc32: hasher_class = "intel_crc32_intrinsic_hasher"; break;
        case hash_strategy::lehmer: hasher_class = "lehmer_hasher"; break;
        case hash_strategy::lehmer_128: hasher_class = "lehmer_128_hasher"; break;
    }
    std::fprintf(file, R"(
};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {
    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    %s hasher(hash_basis);
    hash_selected_characters(character_selection, hasher, identifier, size);
    std::uint32_t index = hasher.hash() %% table_size;

    const table_entry& entry = table[index];
    if (std::strncmp(identifier, entry.keyword, size) == 0) {
        return entry.type;
    }
    return token_type::identifier;
}
}
)", hasher_class);

    std::fclose(file);
}

void write_table(const std::string& file_path, const perfect_hash_table& table) {
    write_table(file_path.c_str(), table);
}

table_strategy parse_out_file_path(std::string_view path, const keyword_statistics& stats) {
    constexpr std::string_view prefix = "generated/pht-";
    constexpr std::string_view suffix = ".cpp";
    if (!path.starts_with(prefix)) {
        std::fprintf(stderr, "error: unexpected path: %.*s\n", (int)path.size(), path.data());
        std::exit(1);
    }
    if (!path.ends_with(suffix)) {
        std::fprintf(stderr, "error: unexpected path: %.*s\n", (int)path.size(), path.data());
        std::exit(1);
    }
    path.remove_prefix(prefix.size());
    path.remove_suffix(suffix.size());

    std::vector<std::string_view> parts;
    for (;;) {
        auto hyphen = path.find('-');
        if (hyphen == std::string_view::npos) {
            parts.push_back(path);
            break;
        }
        parts.push_back(path.substr(0, hyphen));
        path = path.substr(hyphen + 1);
    }

    if (parts.size() != 3) {
        std::fprintf(stderr, "error: expected 3 parts in path: %.*s\n", (int)path.size(), path.data());
        std::exit(1);
    }

    auto look_up_or_die = [](std::string_view input, const char* what, const auto& map) -> auto {
        auto it = map.find(input);
        if (it == map.end()) {
            std::fprintf(stderr, "error: cannot parse %s: %.*s", what, (int)input.size(), input.data());
            std::exit(1);
        }
        return it->second;
    };

    table_strategy strategy;
    strategy.size_strategy = look_up_or_die(parts[0], "table strategy", std::map<std::string_view, table_size_strategy>{
        {"small", table_size_strategy::smallest},
        {"pot", table_size_strategy::power_of_2},
    });

    
    std::from_chars_result r = std::from_chars(parts[1].data(), parts[1].data() + parts[1].size(), strategy.character_selection);
    if (r.ptr != parts[1].data() + parts[1].size() || r.ec != std::errc()) {
        std::fprintf(stderr, "error: cannot parse character selection: %.*s", (int)parts[1].size(), parts[1].data());
        std::exit(1);
    }
    if (std::count(stats.unique_character_selections.begin(), stats.unique_character_selections.end(), strategy.character_selection) == 0) {
        std::fprintf(stderr, "error: character selection is not unique: %u", strategy.character_selection);
        std::exit(1);
    }

    strategy.hasher = look_up_or_die(parts[2], "table strategy", std::map<std::string_view, hash_strategy>{
        {"fnv1a32", hash_strategy::fnv1a32},
        {"xx364", hash_strategy::xx3_64},
        {"icrc32", hash_strategy::intel_crc32},
        {"lehmer", hash_strategy::lehmer},
        {"lehmer128", hash_strategy::lehmer_128},
    });

    return strategy;
}

void go(const char* out_file_path) {
    keyword_statistics stats = make_stats();
    table_strategy strategy = parse_out_file_path(out_file_path, stats);
    write_table(out_file_path, make_perfect_hash_table(stats, strategy));
}
}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "error: expected file path\n");
        std::exit(1);
    }
    pht::go(argv[1]);
    return 0;
}

// quick-lint-js finds bugs in JavaScript programs.
// Copyright (C) 2020  Matthew "strager" Glazar
//
// This file is part of quick-lint-js.
//
// quick-lint-js is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// quick-lint-js is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with quick-lint-js.  If not, see <https://www.gnu.org/licenses/>.
