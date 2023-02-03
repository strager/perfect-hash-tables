// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_LEX_H
#define PERFECT_HASH_TABLES_LEX_H

#include <cassert>
#include <cstddef>
#include <vector>

namespace pht {
struct lexed_token {
    const char* begin;
    std::size_t size;
};

inline bool lex_is_identifier_start(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

inline bool lex_is_identifier_cont(char c) {
    return lex_is_identifier_start(c) || ('0' <= c && c <= '9');
}

inline std::vector<lexed_token> lex(const char* code) {
    auto skip_whitespace = [&code]() -> void {
        while (*code != '\0' && !lex_is_identifier_start(*code)) {
            code += 1;
        }
    };

    std::vector<lexed_token> tokens;
    skip_whitespace();
    while (*code != '\0') {
        // Read the identifier token.
        assert(lex_is_identifier_start(*code));
        const char* begin = code;
        code += 1;
        while (lex_is_identifier_cont(*code)) {
            code += 1;
        }
        tokens.push_back(lexed_token{begin, static_cast<std::size_t>(code - begin)});

        skip_whitespace();
    }
    return tokens;
}
}

#endif

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
