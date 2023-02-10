/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf --jump=1 token.gperf  */
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
/* maximum key range = 102, duplicates = 0 */

class lexer_keyword
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static struct keyword_entry *look_up (const char *str, size_t len);
};

inline unsigned int
lexer_keyword::hash (const char *str, size_t len)
{
  static unsigned char asso_values[] =
    {
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107,  15,  72,  13,
       41,   2,   9,  43,  55,   5, 107,  25,  22,  74,
        2,  19,  50, 107,   4,   0,   0,  34,  32,  16,
        0,  51, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
      107, 107, 107, 107, 107, 107
    };
  return len + asso_values[static_cast<unsigned char>(str[1])] + asso_values[static_cast<unsigned char>(str[0])] + asso_values[static_cast<unsigned char>(str[len - 1])];
}

struct keyword_entry *
lexer_keyword::look_up (const char *str, size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 80,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 11,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 106
    };

  static struct keyword_entry wordlist[] =
    {
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"set",         token_type::kw_set},
      {"",static_cast<token_type>(0)},
      {"is",          token_type::kw_is},
      {"export",      token_type::kw_export},
      {"extends",     token_type::kw_extends},
      {"true",        token_type::kw_true},
      {"in",          token_type::kw_in},
      {"",static_cast<token_type>(0)},
      {"never",       token_type::kw_never},
      {"return",      token_type::kw_return},
      {"require",     token_type::kw_require},
      {"infer",       token_type::kw_infer},
      {"as",          token_type::kw_as},
      {"interface",   token_type::kw_interface},
      {"static",      token_type::kw_static},
      {"",static_cast<token_type>(0)},
      {"assert",      token_type::kw_assert},
      {"asserts",     token_type::kw_asserts},
      {"new",         token_type::kw_new},
      {"",static_cast<token_type>(0)},
      {"if",          token_type::kw_if},
      {"instanceof",  token_type::kw_instanceof},
      {"let",         token_type::kw_let},
      {"namespace",   token_type::kw_namespace},
      {"intrinsic",   token_type::kw_intrinsic},
      {"else",        token_type::kw_else},
      {"false",       token_type::kw_false},
      {"",static_cast<token_type>(0)},
      {"async",       token_type::kw_async},
      {"case",        token_type::kw_case},
      {"for",         token_type::kw_for},
      {"await",       token_type::kw_await},
      {"const",       token_type::kw_const},
      {"",static_cast<token_type>(0)},
      {"of",          token_type::kw_of},
      {"class",       token_type::kw_class},
      {"keyof",       token_type::kw_keyof},
      {"continue",    token_type::kw_continue},
      {"super",       token_type::kw_super},
      {"unique",      token_type::kw_unique},
      {"unknown",     token_type::kw_unknown},
      {"number",      token_type::kw_number},
      {"constructor", token_type::kw_constructor},
      {"get",         token_type::kw_get},
      {"string",      token_type::kw_string},
      {"default",     token_type::kw_default},
      {"delete",      token_type::kw_delete},
      {"declare",     token_type::kw_declare},
      {"function",    token_type::kw_function},
      {"var",         token_type::kw_var},
      {"debugger",    token_type::kw_debugger},
      {"out",         token_type::kw_out},
      {"type",        token_type::kw_type},
      {"try",         token_type::kw_try},
      {"this",        token_type::kw_this},
      {"",static_cast<token_type>(0)},
      {"override",    token_type::kw_override},
      {"null",        token_type::kw_null},
      {"private",     token_type::kw_private},
      {"",static_cast<token_type>(0)},
      {"readonly",    token_type::kw_readonly},
      {"typeof",      token_type::kw_typeof},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"any",         token_type::kw_any},
      {"finally",     token_type::kw_finally},
      {"",static_cast<token_type>(0)},
      {"package",     token_type::kw_package},
      {"",static_cast<token_type>(0)},
      {"throw",       token_type::kw_throw},
      {"switch",      token_type::kw_switch},
      {"while",       token_type::kw_while},
      {"symbol",      token_type::kw_symbol},
      {"with",        token_type::kw_with},
      {"do",          token_type::kw_do},
      {"enum",        token_type::kw_enum},
      {"bigint",      token_type::kw_bigint},
      {"",static_cast<token_type>(0)},
      {"import",      token_type::kw_import},
      {"undefined",   token_type::kw_undefined},
      {"",static_cast<token_type>(0)},
      {"catch",       token_type::kw_catch},
      {"implements",  token_type::kw_implements},
      {"",static_cast<token_type>(0)},
      {"from",        token_type::kw_from},
      {"",static_cast<token_type>(0)},
      {"global",      token_type::kw_global},
      {"",static_cast<token_type>(0)},
      {"abstract",    token_type::kw_abstract},
      {"void",        token_type::kw_void},
      {"object",      token_type::kw_object},
      {"",static_cast<token_type>(0)},
      {"",static_cast<token_type>(0)},
      {"boolean",     token_type::kw_boolean},
      {"module",      token_type::kw_module},
      {"yield",       token_type::kw_yield},
      {"public",      token_type::kw_public},
      {"protected",   token_type::kw_protected},
      {"",static_cast<token_type>(0)},
      {"break",       token_type::kw_break}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          const char *s = wordlist[key].string;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
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
