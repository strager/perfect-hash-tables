// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_FILE_H
#define PERFECT_HASH_TABLES_FILE_H

#include "token.h"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace pht {
// Includes padding_bytes null terminators.
inline std::vector<char> read_file(const char* file_path) {
    FILE* file = std::fopen(file_path, "rb");
    if (file == nullptr) {
        std::fprintf(stderr, "error: failed to open %s: %s\n", file_path, std::strerror(errno));
        std::exit(1);
    }

    std::vector<char> result;
    for (;;) {
        char buffer[1024];
        std::size_t rc = std::fread(buffer, 1, sizeof(buffer), file);
        if (std::ferror(file)) {
            std::fprintf(stderr, "error: failed to read %s: %s\n", file_path, std::strerror(errno));
            std::exit(1);
        }
        if (rc == 0) {
            break;
        }
        std::size_t old_size = result.size();
        result.resize(old_size + rc);
        std::memcpy(result.data() + old_size, buffer, rc);
    }
    result.resize(result.size() + padding_bytes);

    std::fclose(file);

    return result;
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
