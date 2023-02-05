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
#include <getopt.h>
#include <map>
#include <optional>
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
    bool inline_hash;
    bool early_check_first_character;
};

struct keyword_statistics {
    unsigned long min_keyword_size;
    unsigned long max_keyword_size;
    std::vector<character_selection_mask> unique_character_selections;
};

struct perfect_hash_table {
    struct table_entry {
        const char* keyword = nullptr;
        std::uint32_t hash = 0;
    };

    keyword_statistics stats;
    unsigned long hash_basis;
    character_selection_mask character_selection;
    hash_strategy hasher;
    unsigned long table_size;
    bool inline_hash;
    bool early_check_first_character;

    std::vector<table_entry> entries;
};

template <class Hasher>
bool try_add_all_entries(perfect_hash_table& table) {
    table.entries.clear();
    table.entries.resize(table.table_size);

    for (keyword_token kt : keyword_tokens) {
        Hasher hasher(table.hash_basis);
        hash_selected_characters(table.character_selection, hasher, kt.keyword, std::strlen(kt.keyword));
        std::uint32_t h = hasher.hash();
        std::uint32_t index = h % table.table_size;
        perfect_hash_table::table_entry &entry = table.entries[index];
        bool taken = entry.keyword != nullptr;
        if (taken) {
            return false;
        }
        entry.keyword = kt.keyword;
        entry.hash = h;
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
    table.inline_hash = strategy.inline_hash;
    table.early_check_first_character = strategy.early_check_first_character;

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
)");
    if (table.inline_hash) {
        std::fprintf(file, R"(
    std::uint32_t hash;
)");
    }
    std::fprintf(file, "%s", R"(
    const char keyword[max_keyword_size + 1];
    token_type type;
};

constexpr table_entry table[table_size] = {
)");

    for (const perfect_hash_table::table_entry& entry : table.entries) {
        std::fprintf(file, "  {");
        if (table.inline_hash) {
            std::fprintf(file, "%luU, ", (unsigned long)entry.hash);
        }
        if (entry.keyword == nullptr) {
            std::fprintf(file, "\"\", token_type::identifier},\n");
        } else {
            std::fprintf(file, "\"%s\", token_type::kw_%s},\n", entry.keyword, entry.keyword);
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
    std::uint32_t h = hasher.hash();
    std::uint32_t index = h %% table_size;

    const table_entry& entry = table[index];
)", hasher_class);
    if (table.inline_hash) {
        std::fprintf(file, "%s", R"(
    if (h != entry.hash) {
        return token_type::identifier;
    }
)");
    }
    if (table.early_check_first_character) {
        std::fprintf(file, "%s", R"(
    if (entry.keyword[0] != identifier[0]) {
        return token_type::identifier;
    }
    if (std::strncmp(identifier + 1, entry.keyword + 1, size) == 0) {
        return entry.type;
    }
)");
    } else {
        std::fprintf(file, "%s", R"(
    if (std::strncmp(identifier, entry.keyword, size) == 0) {
        return entry.type;
    }
)");
    }
    std::fprintf(file, "%s", R"(
    return token_type::identifier;
}
}
)");

    std::fclose(file);
}

void go(int argc, char** argv) {
    auto look_up_or_die = [](const char* input, const char* what, const auto& map) -> auto {
        auto it = map.find(input);
        if (it == map.end()) {
            std::fprintf(stderr, "error: cannot parse %s: %s", what, input);
            std::exit(1);
        }
        return it->second;
    };

    static constexpr ::option long_options[] = {
        {"characters",  required_argument, 0, 'c' },
        {"check-first", no_argument,       0, '1' },
        {"hasher",      required_argument, 0, 'h' },
        {"output",      required_argument, 0, 'o' },
        {"table-size",  required_argument, 0, 't' },
        {"inline-hash", no_argument,       0, 'i' },
        {nullptr,       0,                 0, 0   }
    };

    const char* out_file_path = nullptr;
    bool inline_hash = false;
    bool early_check_first_character = false;
    std::optional<table_size_strategy> size_strategy;
    std::optional<character_selection_mask> character_selection;
    std::optional<hash_strategy> hasher;

    for (;;) {
        int long_index = 0;
        int opt = getopt_long_only(argc, argv, "", long_options, &long_index);
        switch (opt) {
            case 'c': {
                character_selection.emplace();
                const char* optarg_end = optarg + std::strlen(optarg);
                std::from_chars_result r = std::from_chars(optarg, optarg_end, *character_selection);
                if (r.ptr != optarg_end || r.ec != std::errc()) {
                    std::fprintf(stderr, "error: cannot parse character selection: %s", optarg);
                    std::exit(1);
                }
                break;
            }

            case '1':
                early_check_first_character = true;
                break;

            case 'h':
                hasher = look_up_or_die(optarg, "--hasher", std::map<std::string_view, hash_strategy>{
                    {"fnv1a32", hash_strategy::fnv1a32},
                    {"xx364", hash_strategy::xx3_64},
                    {"icrc32", hash_strategy::intel_crc32},
                    {"lehmer", hash_strategy::lehmer},
                    {"lehmer128", hash_strategy::lehmer_128},
                });
                break;

            case 'i':
                inline_hash = true;
                break;

            case 't':
                size_strategy = look_up_or_die(optarg, "--table-size", std::map<std::string_view, table_size_strategy>{
                    {"small", table_size_strategy::smallest},
                    {"pot", table_size_strategy::power_of_2},
                });
                break;

            case 'o':
                out_file_path = optarg;
                break;

            case -1:
                goto done_parsing;

            default:
                std::fprintf(stderr, "error: invalid option\n");
                std::exit(1);
                break;
        }
    }
done_parsing:

    if (!character_selection.has_value()) {
        std::fprintf(stderr, "error: missing required --characters\n");
        std::exit(1);
    }
    if (!hasher.has_value()) {
        std::fprintf(stderr, "error: missing required --hasher\n");
        std::exit(1);
    }
    if (!size_strategy.has_value()) {
        std::fprintf(stderr, "error: missing required --table-size\n");
        std::exit(1);
    }
    table_strategy strategy = {
        .size_strategy = *size_strategy,
        .character_selection = *character_selection,
        .hasher = *hasher,
        .inline_hash = inline_hash,
        .early_check_first_character = early_check_first_character,
    };

    keyword_statistics stats = make_stats();
    if (std::count(stats.unique_character_selections.begin(), stats.unique_character_selections.end(), strategy.character_selection) == 0) {
        std::fprintf(stderr, "error: character selection is not unique: %u", strategy.character_selection);
        std::exit(1);
    }

    write_table(out_file_path, make_perfect_hash_table(stats, strategy));
}
}
}

int main(int argc, char** argv) {
    pht::go(argc, argv);
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
