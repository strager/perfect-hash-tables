// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "token.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace pht {
namespace {
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;

struct table_entry {
    std::string_view keyword;
    token_type type;
};

std::vector<table_entry> make_keyword_table() {
    std::vector<table_entry> result;
    result.reserve(std::size(keyword_tokens));
    for (keyword_token kt : keyword_tokens) {
        table_entry entry;
        entry.keyword = kt.keyword;
        entry.type = kt.type;
        result.push_back(entry);
    }
    return result;
}
}

std::vector<table_entry> keywords = make_keyword_table();

extern "C" 
token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {
    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    std::string_view ident(identifier, size);
    auto it = std::find_if(
        keywords.begin(), keywords.end(),
        [&](const table_entry& entry) -> bool {
            return entry.keyword == ident;
            });
    if (it == keywords.end()) {
        return token_type::identifier;
    } else {
        return it->type;
    }
}
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
