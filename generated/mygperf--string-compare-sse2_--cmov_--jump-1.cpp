#include <cstddef>
#include <cstring>
#include "token.h"
#include <nmmintrin.h>

namespace pht {
namespace {
    constexpr int       TOTAL_KEYWORDS = 80;
constexpr int       MIN_WORD_LENGTH = 2;
constexpr int       MAX_WORD_LENGTH = 11;
constexpr int       MIN_HASH_VALUE = 5;
constexpr int       MAX_HASH_VALUE = 106    ;struct keyword_entry {
  char keyword[MAX_WORD_LENGTH + 1];
  token_type type;
};
}

token_type look_up_identifier(const char* str, std::size_t len) noexcept {
    static constexpr std::uint8_t asso_values[] =     {      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107,  15,  72,  13,       41,   2,   9,  43,  55,   5, 107,  25,  22,  74,        2,  19,  50, 107,   4,   0,   0,  34,  32,  16,        0,  51, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107, 107, 107, 107, 107,      107, 107, 107, 107, 107, 107    };;
    static constexpr keyword_entry wordlist[] =     {      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"set",         token_type::kw_set},      {"",static_cast<token_type>(0)},      {"is",          token_type::kw_is},      {"export",      token_type::kw_export},      {"extends",     token_type::kw_extends},      {"true",        token_type::kw_true},      {"in",          token_type::kw_in},      {"",static_cast<token_type>(0)},      {"never",       token_type::kw_never},      {"return",      token_type::kw_return},      {"require",     token_type::kw_require},      {"infer",       token_type::kw_infer},      {"as",          token_type::kw_as},      {"interface",   token_type::kw_interface},      {"static",      token_type::kw_static},      {"",static_cast<token_type>(0)},      {"assert",      token_type::kw_assert},      {"asserts",     token_type::kw_asserts},      {"new",         token_type::kw_new},      {"",static_cast<token_type>(0)},      {"if",          token_type::kw_if},      {"instanceof",  token_type::kw_instanceof},      {"let",         token_type::kw_let},      {"namespace",   token_type::kw_namespace},      {"intrinsic",   token_type::kw_intrinsic},      {"else",        token_type::kw_else},      {"false",       token_type::kw_false},      {"",static_cast<token_type>(0)},      {"async",       token_type::kw_async},      {"case",        token_type::kw_case},      {"for",         token_type::kw_for},      {"await",       token_type::kw_await},      {"const",       token_type::kw_const},      {"",static_cast<token_type>(0)},      {"of",          token_type::kw_of},      {"class",       token_type::kw_class},      {"keyof",       token_type::kw_keyof},      {"continue",    token_type::kw_continue},      {"super",       token_type::kw_super},      {"unique",      token_type::kw_unique},      {"unknown",     token_type::kw_unknown},      {"number",      token_type::kw_number},      {"constructor", token_type::kw_constructor},      {"get",         token_type::kw_get},      {"string",      token_type::kw_string},      {"default",     token_type::kw_default},      {"delete",      token_type::kw_delete},      {"declare",     token_type::kw_declare},      {"function",    token_type::kw_function},      {"var",         token_type::kw_var},      {"debugger",    token_type::kw_debugger},      {"out",         token_type::kw_out},      {"type",        token_type::kw_type},      {"try",         token_type::kw_try},      {"this",        token_type::kw_this},      {"",static_cast<token_type>(0)},      {"override",    token_type::kw_override},      {"null",        token_type::kw_null},      {"private",     token_type::kw_private},      {"",static_cast<token_type>(0)},      {"readonly",    token_type::kw_readonly},      {"typeof",      token_type::kw_typeof},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"any",         token_type::kw_any},      {"finally",     token_type::kw_finally},      {"",static_cast<token_type>(0)},      {"package",     token_type::kw_package},      {"",static_cast<token_type>(0)},      {"throw",       token_type::kw_throw},      {"switch",      token_type::kw_switch},      {"while",       token_type::kw_while},      {"symbol",      token_type::kw_symbol},      {"with",        token_type::kw_with},      {"do",          token_type::kw_do},      {"enum",        token_type::kw_enum},      {"bigint",      token_type::kw_bigint},      {"",static_cast<token_type>(0)},      {"import",      token_type::kw_import},      {"undefined",   token_type::kw_undefined},      {"",static_cast<token_type>(0)},      {"catch",       token_type::kw_catch},      {"implements",  token_type::kw_implements},      {"",static_cast<token_type>(0)},      {"from",        token_type::kw_from},      {"",static_cast<token_type>(0)},      {"global",      token_type::kw_global},      {"",static_cast<token_type>(0)},      {"abstract",    token_type::kw_abstract},      {"void",        token_type::kw_void},      {"object",      token_type::kw_object},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"boolean",     token_type::kw_boolean},      {"module",      token_type::kw_module},      {"yield",       token_type::kw_yield},      {"public",      token_type::kw_public},      {"protected",   token_type::kw_protected},      {"",static_cast<token_type>(0)},      {"break",       token_type::kw_break}    };;

    if (len > MAX_WORD_LENGTH || len < MIN_WORD_LENGTH) {
        return token_type::identifier;
    }

    unsigned h =   len + asso_values[static_cast<unsigned char>(str[1])] + asso_values[static_cast<unsigned char>(str[0])] + asso_values[static_cast<unsigned char>(str[len - 1])];
    if (h > MAX_HASH_VALUE) {
        return token_type::identifier;
    }

    const keyword_entry& entry = wordlist[h];
    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry.keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)str);
    // Calculating the mask this way seems to be much much faster than '(1 << len) - 1'.
    std::uint32_t inv_mask = ~(std::uint32_t)0 << len;
    std::uint32_t mask = ~inv_mask;
    std::uint32_t equal_mask = ::_mm_movemask_epi8(::_mm_cmpeq_epi8(entry_unmasked, identifier_unmasked));
    std::uint32_t not_equal_mask = ~equal_mask;
    int result = (int)entry.type;
    __asm__(
        // If what should be the null terminator is not null, then
        // (len != strlen(entry.keyword)), so set result to
        // token_type::identifier.
        "cmpb $0, %[entry_keyword_at_size]\n"
        "cmovne %[token_type_identifier], %[result]\n"

        : [result]"+r"(result)

        : [entry_keyword_at_size]"m"(entry.keyword[len]),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by cmp.
    );

    __asm__(
        "test %[not_equal_mask], %[mask]\n"
        "cmovne %[token_type_identifier], %[result]\n"

        : [result]"+r"(result)

        : [not_equal_mask]"r"(not_equal_mask),
          [mask]"r"(mask),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by test.
    );
    return (token_type)result;
}
}
