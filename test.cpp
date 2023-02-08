// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "implementations.h"
#include "token.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>

namespace pht {
namespace {
void test_look_up_identifier(const char* name, look_up_identifier_f* look_up_identifier) {
    struct test_case {
        char identifier[20 + padding_bytes];
        token_type expected;
    };

    static constexpr test_case test_cases[] = {
        {"hello", token_type::identifier},
        {"___", token_type::identifier},
        {"abstractt", token_type::identifier},
        {"AS", token_type::identifier},
        {"asx", token_type::identifier},
        {"as0", token_type::identifier},
        {"_as", token_type::identifier},

        PHT_KEYWORD_TOKENS_TABLE
    };

    bool ok = true;
    for (const test_case& test : test_cases) {
        std::size_t length = std::strlen(test.identifier);
        token_type actual = look_up_identifier(test.identifier, length);
        if (actual != test.expected) {
            std::fprintf(
                stderr,
                "FAIL: %s: expected look_up_identifier(\"%s\", %zu) = %d, but got %d\n",
                name,
                test.identifier,
                length,
                (int) test.expected,
                (int) actual);
            ok = false;
        }
    }
    if (!ok) {
        std::exit(1);
    }

    std::fprintf(stderr, "OK: %s\n", name);
}
}
}

int main(int argc, char** argv) {
    const char* filter = argc > 1 ? argv[1] : nullptr;
    std::array<pht::implementation, pht::implementation_count> implementations
        = pht::load_implementations();
    for (pht::implementation impl : implementations) {
        if (filter == nullptr || std::strstr(impl.name, filter) != nullptr) {
            pht::test_look_up_identifier(impl.name, impl.look_up_identifier);
        }
    }
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
