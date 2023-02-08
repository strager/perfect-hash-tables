/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf --compare-lengths --readonly-tables token.gperf  */
/* Computed positions: -k'1-2,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif


// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include <stddef.h>
#include <string.h>
#include "token.h"

namespace pht {
namespace {
constexpr int max_keyword_length = 11;

struct keyword_entry {
#if GPERF_PIC
  int string;
#elif GPERF_INLINE
  char string[max_keyword_length + 1];
#else
  const char* string;
#endif
  token_type type;
};
/* maximum key range = 160, duplicates = 0 */

class lexer_keyword
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static const struct keyword_entry *look_up (const char *str, size_t len);
};

inline unsigned int
lexer_keyword::hash (const char *str, size_t len)
{
  static const unsigned char asso_values[] =
    {
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162,  20,  85,   0,
       60,   5,  15,   5,  30,   0, 162,  60,  75,  60,
        5,  30,  70, 162,   0,   0,   0,  35,  10,  25,
       25,  75, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
      162, 162, 162, 162, 162, 162
    };
  return len + asso_values[static_cast<unsigned char>(str[1])] + asso_values[static_cast<unsigned char>(str[0])] + asso_values[static_cast<unsigned char>(str[len - 1])];
}

const struct keyword_entry *
lexer_keyword::look_up (const char *str, size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 80,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 11,
      MIN_HASH_VALUE = 2,
      MAX_HASH_VALUE = 161
    };

  static const unsigned char lengthtable[] =
    {
       0,  0,  2,  0,  0,  0,  6,  0,  3,  4,  5,  6,  2,  3,
       9,  5,  6,  7,  0,  9,  0,  0,  2,  0,  0,  5,  6,  7,
       0,  4, 10,  0,  2,  3,  4,  5,  6,  7,  3,  9,  5, 11,
       0,  8,  0,  5,  6,  0,  3,  0,  5,  6,  7,  8,  0,  5,
       0,  0,  0,  4,  5,  6,  2,  8,  0,  5,  6,  0,  3,  0,
      10,  0,  7,  8,  4,  0,  6,  7,  3,  4,  5,  0,  7,  3,
       4,  5,  0,  0,  8,  4,  0,  6,  0,  0,  0,  0,  6,  7,
       0,  0,  0,  6,  7,  3,  4,  0,  0,  0,  0,  9,  0,  6,
       0,  8,  0,  0,  0,  0,  0,  4,  0,  6,  2,  0,  0,  0,
       0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,
       5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,
       0,  0,  6,  0,  0,  0,  0,  6
    };
  static const struct keyword_entry wordlist[] =
    {
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"is",          token_type::kw_is},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"static",      token_type::kw_static},
      {"",static_cast<token_type>(0)},
      {"set",         token_type::kw_set},
      {"true",        token_type::kw_true},
      {"infer",       token_type::kw_infer},
      {"string",      token_type::kw_string},
      {"in",          token_type::kw_in},
      {"get",         token_type::kw_get},
      {"intrinsic",   token_type::kw_intrinsic},
      {"never",       token_type::kw_never},
      {"return",      token_type::kw_return},
      {"require",     token_type::kw_require},
      {"",static_cast<token_type>(0)},
      {"interface",   token_type::kw_interface},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"as",          token_type::kw_as},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"async",       token_type::kw_async},
      {"assert",      token_type::kw_assert},
      {"asserts",     token_type::kw_asserts},
      {"",static_cast<token_type>(0)},
      {"case",        token_type::kw_case},
      {"instanceof",  token_type::kw_instanceof},
      {"",static_cast<token_type>(0)},
      {"if",          token_type::kw_if},
      {"var",         token_type::kw_var},
      {"this",        token_type::kw_this},
      {"const",       token_type::kw_const},
      {"export",      token_type::kw_export},
      {"extends",     token_type::kw_extends},
      {"new",         token_type::kw_new},
      {"namespace",   token_type::kw_namespace},
      {"super",       token_type::kw_super},
      {"constructor", token_type::kw_constructor},
      {"",static_cast<token_type>(0)},
      {"continue",    token_type::kw_continue},
      {"",static_cast<token_type>(0)},
      {"false",       token_type::kw_false},
      {"number",      token_type::kw_number},
      {"",static_cast<token_type>(0)},
      {"for",         token_type::kw_for},
      {"",static_cast<token_type>(0)},
      {"await",       token_type::kw_await},
      {"unique",      token_type::kw_unique},
      {"unknown",     token_type::kw_unknown},
      {"override",    token_type::kw_override},
      {"",static_cast<token_type>(0)},
      {"catch",       token_type::kw_catch},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"with",        token_type::kw_with},
      {"throw",       token_type::kw_throw},
      {"switch",      token_type::kw_switch},
      {"of",          token_type::kw_of},
      {"function",    token_type::kw_function},
      {"",static_cast<token_type>(0)},
      {"while",       token_type::kw_while},
      {"import",      token_type::kw_import},
      {"",static_cast<token_type>(0)},
      {"out",         token_type::kw_out},
      {"",static_cast<token_type>(0)},
      {"implements",  token_type::kw_implements},
      {"",static_cast<token_type>(0)},
      {"default",     token_type::kw_default},
      {"debugger",    token_type::kw_debugger},
      {"enum",        token_type::kw_enum},
      {"",static_cast<token_type>(0)},
      {"delete",      token_type::kw_delete},
      {"declare",     token_type::kw_declare},
      {"try",         token_type::kw_try},
      {"from",        token_type::kw_from},
      {"class",       token_type::kw_class},
      {"",static_cast<token_type>(0)},
      {"private",     token_type::kw_private},
      {"let",         token_type::kw_let},
      {"type",        token_type::kw_type},
      {"keyof",       token_type::kw_keyof},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"readonly",    token_type::kw_readonly},
      {"else",        token_type::kw_else},
      {"",static_cast<token_type>(0)},
      {"bigint",      token_type::kw_bigint},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"typeof",      token_type::kw_typeof},
      {"finally",     token_type::kw_finally},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"module",      token_type::kw_module},
      {"package",     token_type::kw_package},
      {"any",         token_type::kw_any},
      {"void",        token_type::kw_void},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"undefined",   token_type::kw_undefined},
      {"",static_cast<token_type>(0)},
      {"public",      token_type::kw_public},
      {"",static_cast<token_type>(0)},
      {"abstract",    token_type::kw_abstract},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"null",        token_type::kw_null},
      {"",static_cast<token_type>(0)},
      {"object",      token_type::kw_object},
      {"do",          token_type::kw_do},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"boolean",     token_type::kw_boolean},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"protected",   token_type::kw_protected},
      {"yield",       token_type::kw_yield},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"break",       token_type::kw_break},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"symbol",      token_type::kw_symbol},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"global",      token_type::kw_global}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        if (len == lengthtable[key])
          {
            const char *s = wordlist[key].string;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}

}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {
  const keyword_entry *entry = lexer_keyword::look_up(identifier, size);
  if (entry) {
    return entry->type;
  } else {
    return token_type::identifier;
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
