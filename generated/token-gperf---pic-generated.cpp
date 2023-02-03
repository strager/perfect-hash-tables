/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf --pic token.gperf  */
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
struct keyword_entry {
#if GPERF_PIC
  int string;
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
  static struct keyword_entry *look_up (const char *str, size_t len);
};

inline unsigned int
lexer_keyword::hash (const char *str, size_t len)
{
  static unsigned char asso_values[] =
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

struct stringpool_t
  {
    char stringpool_str2[sizeof("is")];
    char stringpool_str6[sizeof("static")];
    char stringpool_str8[sizeof("set")];
    char stringpool_str9[sizeof("true")];
    char stringpool_str10[sizeof("infer")];
    char stringpool_str11[sizeof("string")];
    char stringpool_str12[sizeof("in")];
    char stringpool_str13[sizeof("get")];
    char stringpool_str14[sizeof("intrinsic")];
    char stringpool_str15[sizeof("never")];
    char stringpool_str16[sizeof("return")];
    char stringpool_str17[sizeof("require")];
    char stringpool_str19[sizeof("interface")];
    char stringpool_str22[sizeof("as")];
    char stringpool_str25[sizeof("async")];
    char stringpool_str26[sizeof("assert")];
    char stringpool_str27[sizeof("asserts")];
    char stringpool_str29[sizeof("case")];
    char stringpool_str30[sizeof("instanceof")];
    char stringpool_str32[sizeof("if")];
    char stringpool_str33[sizeof("var")];
    char stringpool_str34[sizeof("this")];
    char stringpool_str35[sizeof("const")];
    char stringpool_str36[sizeof("export")];
    char stringpool_str37[sizeof("extends")];
    char stringpool_str38[sizeof("new")];
    char stringpool_str39[sizeof("namespace")];
    char stringpool_str40[sizeof("super")];
    char stringpool_str41[sizeof("constructor")];
    char stringpool_str43[sizeof("continue")];
    char stringpool_str45[sizeof("false")];
    char stringpool_str46[sizeof("number")];
    char stringpool_str48[sizeof("for")];
    char stringpool_str50[sizeof("await")];
    char stringpool_str51[sizeof("unique")];
    char stringpool_str52[sizeof("unknown")];
    char stringpool_str53[sizeof("override")];
    char stringpool_str55[sizeof("catch")];
    char stringpool_str59[sizeof("with")];
    char stringpool_str60[sizeof("throw")];
    char stringpool_str61[sizeof("switch")];
    char stringpool_str62[sizeof("of")];
    char stringpool_str63[sizeof("function")];
    char stringpool_str65[sizeof("while")];
    char stringpool_str66[sizeof("import")];
    char stringpool_str68[sizeof("out")];
    char stringpool_str70[sizeof("implements")];
    char stringpool_str72[sizeof("default")];
    char stringpool_str73[sizeof("debugger")];
    char stringpool_str74[sizeof("enum")];
    char stringpool_str76[sizeof("delete")];
    char stringpool_str77[sizeof("declare")];
    char stringpool_str78[sizeof("try")];
    char stringpool_str79[sizeof("from")];
    char stringpool_str80[sizeof("class")];
    char stringpool_str82[sizeof("private")];
    char stringpool_str83[sizeof("let")];
    char stringpool_str84[sizeof("type")];
    char stringpool_str85[sizeof("keyof")];
    char stringpool_str88[sizeof("readonly")];
    char stringpool_str89[sizeof("else")];
    char stringpool_str91[sizeof("bigint")];
    char stringpool_str96[sizeof("typeof")];
    char stringpool_str97[sizeof("finally")];
    char stringpool_str101[sizeof("module")];
    char stringpool_str102[sizeof("package")];
    char stringpool_str103[sizeof("any")];
    char stringpool_str104[sizeof("void")];
    char stringpool_str109[sizeof("undefined")];
    char stringpool_str111[sizeof("public")];
    char stringpool_str113[sizeof("abstract")];
    char stringpool_str119[sizeof("null")];
    char stringpool_str121[sizeof("object")];
    char stringpool_str122[sizeof("do")];
    char stringpool_str127[sizeof("boolean")];
    char stringpool_str139[sizeof("protected")];
    char stringpool_str140[sizeof("yield")];
    char stringpool_str150[sizeof("break")];
    char stringpool_str156[sizeof("symbol")];
    char stringpool_str161[sizeof("global")];
  };
static struct stringpool_t stringpool_contents =
  {
    "is",
    "static",
    "set",
    "true",
    "infer",
    "string",
    "in",
    "get",
    "intrinsic",
    "never",
    "return",
    "require",
    "interface",
    "as",
    "async",
    "assert",
    "asserts",
    "case",
    "instanceof",
    "if",
    "var",
    "this",
    "const",
    "export",
    "extends",
    "new",
    "namespace",
    "super",
    "constructor",
    "continue",
    "false",
    "number",
    "for",
    "await",
    "unique",
    "unknown",
    "override",
    "catch",
    "with",
    "throw",
    "switch",
    "of",
    "function",
    "while",
    "import",
    "out",
    "implements",
    "default",
    "debugger",
    "enum",
    "delete",
    "declare",
    "try",
    "from",
    "class",
    "private",
    "let",
    "type",
    "keyof",
    "readonly",
    "else",
    "bigint",
    "typeof",
    "finally",
    "module",
    "package",
    "any",
    "void",
    "undefined",
    "public",
    "abstract",
    "null",
    "object",
    "do",
    "boolean",
    "protected",
    "yield",
    "break",
    "symbol",
    "global"
  };
#define stringpool ((const char *) &stringpool_contents)
struct keyword_entry *
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

  static struct keyword_entry wordlist[] =
    {
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str2,          token_type::kw_is},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str6,      token_type::kw_static},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str8,         token_type::kw_set},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str9,        token_type::kw_true},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str10,       token_type::kw_infer},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str11,      token_type::kw_string},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str12,          token_type::kw_in},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str13,         token_type::kw_get},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str14,   token_type::kw_intrinsic},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str15,       token_type::kw_never},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str16,      token_type::kw_return},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str17,     token_type::kw_require},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str19,   token_type::kw_interface},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str22,          token_type::kw_as},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str25,       token_type::kw_async},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str26,      token_type::kw_assert},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str27,     token_type::kw_asserts},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str29,        token_type::kw_case},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str30,  token_type::kw_instanceof},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str32,          token_type::kw_if},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str33,         token_type::kw_var},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str34,        token_type::kw_this},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str35,       token_type::kw_const},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str36,      token_type::kw_export},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str37,     token_type::kw_extends},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str38,         token_type::kw_new},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str39,   token_type::kw_namespace},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str40,       token_type::kw_super},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str41, token_type::kw_constructor},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str43,    token_type::kw_continue},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str45,       token_type::kw_false},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str46,      token_type::kw_number},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str48,         token_type::kw_for},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str50,       token_type::kw_await},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str51,      token_type::kw_unique},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str52,     token_type::kw_unknown},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str53,    token_type::kw_override},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str55,       token_type::kw_catch},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str59,        token_type::kw_with},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str60,       token_type::kw_throw},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str61,      token_type::kw_switch},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str62,          token_type::kw_of},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str63,    token_type::kw_function},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str65,       token_type::kw_while},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str66,      token_type::kw_import},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str68,         token_type::kw_out},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str70,  token_type::kw_implements},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str72,     token_type::kw_default},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str73,    token_type::kw_debugger},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str74,        token_type::kw_enum},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str76,      token_type::kw_delete},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str77,     token_type::kw_declare},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str78,         token_type::kw_try},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str79,        token_type::kw_from},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str80,       token_type::kw_class},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str82,     token_type::kw_private},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str83,         token_type::kw_let},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str84,        token_type::kw_type},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str85,       token_type::kw_keyof},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str88,    token_type::kw_readonly},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str89,        token_type::kw_else},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str91,      token_type::kw_bigint},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str96,      token_type::kw_typeof},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str97,     token_type::kw_finally},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str101,      token_type::kw_module},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str102,     token_type::kw_package},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str103,         token_type::kw_any},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str104,        token_type::kw_void},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str109,   token_type::kw_undefined},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str111,      token_type::kw_public},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str113,    token_type::kw_abstract},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str119,        token_type::kw_null},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str121,      token_type::kw_object},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str122,          token_type::kw_do},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str127,     token_type::kw_boolean},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str139,   token_type::kw_protected},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str140,       token_type::kw_yield},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str150,       token_type::kw_break},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str156,      token_type::kw_symbol},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {-1,static_cast<token_type>(0)},
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str161,      token_type::kw_global}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          int o = wordlist[key].string;
          if (o >= 0)
            {
              const char *s = o + stringpool;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &wordlist[key];
            }
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
