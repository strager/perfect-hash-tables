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
