// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "pht.h"
#include "token.h"
#include <algorithm>
#include <bit>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace pht {
namespace {
enum class table_size_strategy {
    five_x,
    five_x_next_power_of_2,
};

struct table_strategy {
    table_size_strategy size_strategy;
};

struct perfect_hash_table {
    struct table_entry {
        const char* keyword = nullptr;
    };

    unsigned long hash_basis;
    unsigned long table_size;
    unsigned long min_keyword_size;
    unsigned long max_keyword_size;

    std::vector<table_entry> entries;
};

bool try_add_all_entries(perfect_hash_table& table) {
    table.entries.resize(table.table_size);
    for (perfect_hash_table::table_entry& entry : table.entries) {
        entry.keyword = nullptr;
    }

    for (keyword_token kt : keyword_tokens) {
        std::uint32_t h = hash(table.hash_basis, kt.keyword, std::strlen(kt.keyword));
        std::uint32_t index = h % table.table_size;
        bool taken = table.entries[index].keyword != nullptr;
        if (taken) {
            return false;
        }
        table.entries[index].keyword = kt.keyword;
    }

    return true;
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
        bool succeeded = try_add_all_entries(table);
        if (succeeded) {
            return attempts;
        }
        table.hash_basis += 1;
    }
}

perfect_hash_table make_perfect_hash_table(table_strategy strategy) {
    perfect_hash_table table;
    switch (strategy.size_strategy) {
        case table_size_strategy::five_x:
            table.table_size = std::size(keyword_tokens) * 5;
            break;
        case table_size_strategy::five_x_next_power_of_2:
            table.table_size = std::bit_ceil(std::size(keyword_tokens) * 5);
            break;
    }

    table.min_keyword_size = 0xffffffff;
    table.max_keyword_size = 0;
    for (keyword_token kt : keyword_tokens) {
        unsigned long size = std::strlen(kt.keyword);
        table.min_keyword_size = std::min(table.min_keyword_size, size);
        table.max_keyword_size = std::max(table.max_keyword_size, size);
    }


    int max_attempts = 1'000'000;
    int attempts = try_build_table(table, max_attempts);
    if (attempts >= max_attempts) {
        std::fprintf(
            stderr,
            "can't generate table of size %lu from %zu items after %d attempts\n",
            table.table_size,
            std::size(keyword_tokens),
            attempts);
        std::exit(1);
    }
    std::fprintf(
        stderr,
        "took %d attempts to generate table of size %lu from %zu items\n",
        attempts,
        table.table_size,
        std::size(keyword_tokens));

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
constexpr std::uint32_t hash_basis = %luUL;
constexpr std::uint32_t table_size = %luUL;
constexpr std::size_t min_keyword_size = %lu;
constexpr std::size_t max_keyword_size = %lu;

)", table.hash_basis, table.table_size, table.min_keyword_size, table.max_keyword_size);

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

    std::fprintf(file, "%s", R"(
};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {
    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    std::uint32_t h = hash(hash_basis, identifier, size);
    std::uint32_t index = h % table_size;

    const table_entry& entry = table[index];
    if (std::strncmp(identifier, entry.keyword, size) == 0) {
        return entry.type;
    }
    return token_type::identifier;
}
}
)");

    std::fclose(file);
}

void go() {
    write_table("generated/pht-5x.cpp", make_perfect_hash_table(table_strategy{
        .size_strategy = table_size_strategy::five_x,
    }));
    write_table("generated/pht-5xnpot.cpp", make_perfect_hash_table(table_strategy{
        .size_strategy = table_size_strategy::five_x_next_power_of_2,
    }));
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
