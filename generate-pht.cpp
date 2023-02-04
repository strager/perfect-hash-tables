// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "pht.h"
#include "token.h"
#include <algorithm>
#include <bit>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <string>
#include <thread>
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

    unsigned long max_table_size = std::size(keyword_tokens) * 15;
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

void go() {
    keyword_statistics stats = make_stats();

    std::vector<std::thread> threads;
    for (hash_strategy hasher : {hash_strategy::fnv1a32, hash_strategy::xx3_64}) {
        std::string hasher_tag;
        switch (hasher) {
            case hash_strategy::fnv1a32: hasher_tag = "fnv1a32"; break;
            case hash_strategy::xx3_64: hasher_tag = "xx364"; break;
        }
        for (character_selection_mask character_selection : stats.unique_character_selections) {
            if (std::popcount(character_selection) == 5) {
                // Too expensive. Ignore.
                continue;
            }
            std::string selection_tag = std::to_string(character_selection);
            threads.emplace_back([=]() -> void {
                write_table("generated/pht-small-" + selection_tag + "-" + hasher_tag + ".cpp", make_perfect_hash_table(stats, table_strategy{
                    .size_strategy = table_size_strategy::smallest,
                    .character_selection = character_selection,
                    .hasher = hasher,
                }));
            });
            threads.emplace_back([=]() -> void {
                write_table("generated/pht-pot-" + selection_tag + "-" + hasher_tag + ".cpp", make_perfect_hash_table(stats, table_strategy{
                    .size_strategy = table_size_strategy::power_of_2,
                    .character_selection = character_selection,
                    .hasher = hasher,
                }));
            });
        }
    }

    for (std::thread& t : threads) {
        t.join();
    }
}
}
}

int main() {
    pht::go();
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
