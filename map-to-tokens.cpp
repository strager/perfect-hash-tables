// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "file.h"
#include "lex.h"
#include "token.h"
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>

namespace pht {
void make_file(const char* code, const char* file_path, bool include_keywords, bool include_non_keywords) {
    FILE* file = std::fopen(file_path, "wb");
    if (file == nullptr) {
        std::fprintf(stderr, "error: failed to open %s: %s\n", file_path, std::strerror(errno));
        std::exit(1);
    }

    const char *last_code = code;
    for (lexed_token t : lex(code)) {
        bool is_keyword = look_up_identifier(t.begin, t.size) != token_type::identifier;
        if ((include_keywords && is_keyword) || (include_non_keywords && !is_keyword)) {
            for (std::ptrdiff_t i = 0; i < t.begin - last_code; ++i) {
                char space = ' ';
                std::fwrite(&space, 1, 1, file);
            }
            std::fwrite(t.begin, 1, t.size, file);
            last_code = t.begin + t.size;
        }
    }

    std::fflush(file);
    if (std::ferror(file)) {
        std::fprintf(file, "error: failed to write to %s: %s\n", file_path, std::strerror(errno));
        std::exit(1);
    }
    std::fclose(file);
}

void go() {
    std::vector<char> code = read_file("jquery-3.5.1.js");
    make_file(code.data(), "non-keywords.txt", /*include_keywords=*/false, /*include_non_keywords=*/true);
    make_file(code.data(), "keywords.txt", /*include_keywords=*/true, /*include_non_keywords=*/false);
    make_file(code.data(), "mixed.txt", /*include_keywords=*/true, /*include_non_keywords=*/true);
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
