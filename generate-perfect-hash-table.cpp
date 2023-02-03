// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "fnv.h"
#include "token.h"
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace pht {
namespace {
struct perfect_hash_table {
    struct table_entry {
        const char* keyword = nullptr;
    };

    unsigned long hash(const char* s, std::size_t size) noexcept {
        std::uint32_t h = this->hash_basis;
        fnv1a32_byte(&h, (std::uint8_t)s[0]);
        fnv1a32_byte(&h, (std::uint8_t)s[1]);
        fnv1a32_byte(&h, (std::uint8_t)s[size - 2]);
        fnv1a32_byte(&h, (std::uint8_t)s[size - 1]);
        return h;
    }

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
        std::uint32_t h = table.hash(kt.keyword, std::strlen(kt.keyword));
        std::uint32_t index = h % table.table_size;
        bool taken = table.entries[index].keyword != nullptr;
        if (taken) {
            return false;
        }
        table.entries[index].keyword = kt.keyword;
    }

    return true;
}

perfect_hash_table make_perfect_hash_table() {
    perfect_hash_table table;
    table.table_size = std::size(keyword_tokens) * 5;

    table.min_keyword_size = 0xffffffff;
    table.max_keyword_size = 0;
    for (keyword_token kt : keyword_tokens) {
        unsigned long size = std::strlen(kt.keyword);
        table.min_keyword_size = std::min(table.min_keyword_size, size);
        table.max_keyword_size = std::max(table.max_keyword_size, size);
    }

    table.hash_basis = 0x811c9dc5; // FNV-1a 32-bit basis.
    for (;;) {
        bool succeeded = try_add_all_entries(table);
        if (succeeded) {
            break;
        }
        table.hash_basis += 1;
    }

    return table;
}

void write_table(FILE* file, const perfect_hash_table& table) {
    std::fprintf(file, R"(
#include "../fnv.h"
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
std::uint32_t hash(const char* s, std::size_t size) noexcept {
    static_assert(min_keyword_size >= 2);
    std::uint32_t h = hash_basis;
    // TODO(strager): Play with different character selection schemes.
    fnv1a32_byte(&h, (std::uint8_t)s[0]);
    fnv1a32_byte(&h, (std::uint8_t)s[1]);
    fnv1a32_byte(&h, (std::uint8_t)s[size - 2]);
    fnv1a32_byte(&h, (std::uint8_t)s[size - 1]);
    return h;
}

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

    std::uint32_t h = hash(identifier, size);
    std::uint32_t index = h % table_size;

    const table_entry& entry = table[index];
    if (std::strncmp(identifier, entry.keyword, size) == 0) {
        return entry.type;
    }
    return token_type::identifier;
}
}
)");
}

void go() {
    perfect_hash_table table = make_perfect_hash_table();

    const char* file_path = "generated/perfect-hash-table-generated.cpp";
    FILE* file = std::fopen(file_path, "wb");
    if (file == nullptr) {
        std::fprintf(stderr, "error: failed to open %s: %s\n", file_path, std::strerror(errno));
        std::exit(1);
    }
    write_table(file, table);
    std::fclose(file);
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
