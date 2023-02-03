// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_TOKEN_H
#define PERFECT_HASH_TABLES_TOKEN_H

#include <cstdint>

namespace pht {
enum class token_type : std::uint8_t {
  kw_abstract,
  kw_any,
  kw_as,
  kw_assert,
  kw_asserts,
  kw_async,
  kw_await,
  kw_bigint,
  kw_boolean,
  kw_break,
  kw_case,
  kw_catch,
  kw_class,
  kw_const,
  kw_constructor,
  kw_continue,
  kw_debugger,
  kw_declare,
  kw_default,
  kw_delete,
  kw_do,
  kw_else,
  kw_enum,
  kw_export,
  kw_extends,
  kw_false,
  kw_finally,
  kw_for,
  kw_from,
  kw_function,
  kw_get,
  kw_global,
  kw_if,
  kw_implements,
  kw_import,
  kw_in,
  kw_infer,
  kw_instanceof,
  kw_interface,
  kw_intrinsic,
  kw_is,
  kw_keyof,
  kw_let,
  kw_module,
  kw_namespace,
  kw_never,
  kw_new,
  kw_null,
  kw_number,
  kw_object,
  kw_of,
  kw_out,
  kw_override,
  kw_package,
  kw_private,
  kw_protected,
  kw_public,
  kw_readonly,
  kw_require,
  kw_return,
  kw_set,
  kw_static,
  kw_string,
  kw_super,
  kw_switch,
  kw_symbol,
  kw_this,
  kw_throw,
  kw_true,
  kw_try,
  kw_type,
  kw_typeof,
  kw_undefined,
  kw_unique,
  kw_unknown,
  kw_var,
  kw_void,
  kw_while,
  kw_with,
  kw_yield,

  identifier,
};

struct keyword_token {
    const char* keyword;
    token_type type;
};
constexpr keyword_token keyword_tokens[] = {
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

typedef token_type look_up_identifier_f(const char* identifier, std::size_t size) noexcept;

extern "C" 
__attribute__((visibility("default")))
look_up_identifier_f look_up_identifier;
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
