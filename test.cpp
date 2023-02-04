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
        char identifier[20];
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

        {"abstract", token_type::kw_abstract},
        {"any", token_type::kw_any},
        {"as", token_type::kw_as},
        {"assert", token_type::kw_assert},
        {"asserts", token_type::kw_asserts},
        {"async", token_type::kw_async},
        {"await", token_type::kw_await},
        {"bigint", token_type::kw_bigint},
        {"boolean", token_type::kw_boolean},
        {"break", token_type::kw_break},
        {"case", token_type::kw_case},
        {"catch", token_type::kw_catch},
        {"class", token_type::kw_class},
        {"const", token_type::kw_const},
        {"constructor", token_type::kw_constructor},
        {"continue", token_type::kw_continue},
        {"debugger", token_type::kw_debugger},
        {"declare", token_type::kw_declare},
        {"default", token_type::kw_default},
        {"delete", token_type::kw_delete},
        {"do", token_type::kw_do},
        {"else", token_type::kw_else},
        {"enum", token_type::kw_enum},
        {"export", token_type::kw_export},
        {"extends", token_type::kw_extends},
        {"false", token_type::kw_false},
        {"finally", token_type::kw_finally},
        {"for", token_type::kw_for},
        {"from", token_type::kw_from},
        {"function", token_type::kw_function},
        {"get", token_type::kw_get},
        {"global", token_type::kw_global},
        {"if", token_type::kw_if},
        {"implements", token_type::kw_implements},
        {"import", token_type::kw_import},
        {"in", token_type::kw_in},
        {"infer", token_type::kw_infer},
        {"instanceof", token_type::kw_instanceof},
        {"interface", token_type::kw_interface},
        {"intrinsic", token_type::kw_intrinsic},
        {"is", token_type::kw_is},
        {"keyof", token_type::kw_keyof},
        {"let", token_type::kw_let},
        {"module", token_type::kw_module},
        {"namespace", token_type::kw_namespace},
        {"never", token_type::kw_never},
        {"new", token_type::kw_new},
        {"null", token_type::kw_null},
        {"number", token_type::kw_number},
        {"object", token_type::kw_object},
        {"of", token_type::kw_of},
        {"out", token_type::kw_out},
        {"override", token_type::kw_override},
        {"package", token_type::kw_package},
        {"private", token_type::kw_private},
        {"protected", token_type::kw_protected},
        {"public", token_type::kw_public},
        {"readonly", token_type::kw_readonly},
        {"require", token_type::kw_require},
        {"return", token_type::kw_return},
        {"set", token_type::kw_set},
        {"static", token_type::kw_static},
        {"string", token_type::kw_string},
        {"super", token_type::kw_super},
        {"switch", token_type::kw_switch},
        {"symbol", token_type::kw_symbol},
        {"this", token_type::kw_this},
        {"throw", token_type::kw_throw},
        {"true", token_type::kw_true},
        {"try", token_type::kw_try},
        {"type", token_type::kw_type},
        {"typeof", token_type::kw_typeof},
        {"undefined", token_type::kw_undefined},
        {"unique", token_type::kw_unique},
        {"unknown", token_type::kw_unknown},
        {"var", token_type::kw_var},
        {"void", token_type::kw_void},
        {"while", token_type::kw_while},
        {"with", token_type::kw_with},
        {"yield", token_type::kw_yield},
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
