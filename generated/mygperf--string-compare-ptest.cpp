#include <cstddef>
#include <cstring>
#include "token.h"
#include <nmmintrin.h>

namespace pht {
namespace {
    constexpr int       TOTAL_KEYWORDS = 80;
constexpr int       MIN_WORD_LENGTH = 2;
constexpr int       MAX_WORD_LENGTH = 11;
constexpr int       MIN_HASH_VALUE = 2;
constexpr int       MAX_HASH_VALUE = 161    ;struct keyword_entry {
  char keyword[MAX_WORD_LENGTH + 1];
  token_type type;
};
}

token_type look_up_identifier(const char* str, std::size_t len) noexcept {
    static constexpr std::uint8_t asso_values[] =     {      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162,  20,  85,   0,       60,   5,  15,   5,  30,   0, 162,  60,  75,  60,        5,  30,  70, 162,   0,   0,   0,  35,  10,  25,       25,  75, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162, 162, 162, 162, 162,      162, 162, 162, 162, 162, 162    };;
    static constexpr keyword_entry wordlist[] = {
              {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"is",          token_type::kw_is},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"static",      token_type::kw_static},      {"",static_cast<token_type>(0)},      {"set",         token_type::kw_set},      {"true",        token_type::kw_true},      {"infer",       token_type::kw_infer},      {"string",      token_type::kw_string},      {"in",          token_type::kw_in},      {"get",         token_type::kw_get},      {"intrinsic",   token_type::kw_intrinsic},      {"never",       token_type::kw_never},      {"return",      token_type::kw_return},      {"require",     token_type::kw_require},      {"",static_cast<token_type>(0)},      {"interface",   token_type::kw_interface},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"as",          token_type::kw_as},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"async",       token_type::kw_async},      {"assert",      token_type::kw_assert},      {"asserts",     token_type::kw_asserts},      {"",static_cast<token_type>(0)},      {"case",        token_type::kw_case},      {"instanceof",  token_type::kw_instanceof},      {"",static_cast<token_type>(0)},      {"if",          token_type::kw_if},      {"var",         token_type::kw_var},      {"this",        token_type::kw_this},      {"const",       token_type::kw_const},      {"export",      token_type::kw_export},      {"extends",     token_type::kw_extends},      {"new",         token_type::kw_new},      {"namespace",   token_type::kw_namespace},      {"super",       token_type::kw_super},      {"constructor", token_type::kw_constructor},      {"",static_cast<token_type>(0)},      {"continue",    token_type::kw_continue},      {"",static_cast<token_type>(0)},      {"false",       token_type::kw_false},      {"number",      token_type::kw_number},      {"",static_cast<token_type>(0)},      {"for",         token_type::kw_for},      {"",static_cast<token_type>(0)},      {"await",       token_type::kw_await},      {"unique",      token_type::kw_unique},      {"unknown",     token_type::kw_unknown},      {"override",    token_type::kw_override},      {"",static_cast<token_type>(0)},      {"catch",       token_type::kw_catch},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"with",        token_type::kw_with},      {"throw",       token_type::kw_throw},      {"switch",      token_type::kw_switch},      {"of",          token_type::kw_of},      {"function",    token_type::kw_function},      {"",static_cast<token_type>(0)},      {"while",       token_type::kw_while},      {"import",      token_type::kw_import},      {"",static_cast<token_type>(0)},      {"out",         token_type::kw_out},      {"",static_cast<token_type>(0)},      {"implements",  token_type::kw_implements},      {"",static_cast<token_type>(0)},      {"default",     token_type::kw_default},      {"debugger",    token_type::kw_debugger},      {"enum",        token_type::kw_enum},      {"",static_cast<token_type>(0)},      {"delete",      token_type::kw_delete},      {"declare",     token_type::kw_declare},      {"try",         token_type::kw_try},      {"from",        token_type::kw_from},      {"class",       token_type::kw_class},      {"",static_cast<token_type>(0)},      {"private",     token_type::kw_private},      {"let",         token_type::kw_let},      {"type",        token_type::kw_type},      {"keyof",       token_type::kw_keyof},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"readonly",    token_type::kw_readonly},      {"else",        token_type::kw_else},      {"",static_cast<token_type>(0)},      {"bigint",      token_type::kw_bigint},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"typeof",      token_type::kw_typeof},      {"finally",     token_type::kw_finally},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"module",      token_type::kw_module},      {"package",     token_type::kw_package},      {"any",         token_type::kw_any},      {"void",        token_type::kw_void},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"undefined",   token_type::kw_undefined},      {"",static_cast<token_type>(0)},      {"public",      token_type::kw_public},      {"",static_cast<token_type>(0)},      {"abstract",    token_type::kw_abstract},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"null",        token_type::kw_null},      {"",static_cast<token_type>(0)},      {"object",      token_type::kw_object},      {"do",          token_type::kw_do},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"boolean",     token_type::kw_boolean},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"protected",   token_type::kw_protected},      {"yield",       token_type::kw_yield},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"break",       token_type::kw_break},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"symbol",      token_type::kw_symbol},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"",static_cast<token_type>(0)},      {"global",      token_type::kw_global},
    };

    if (len > MAX_WORD_LENGTH || len < MIN_WORD_LENGTH) {
        return token_type::identifier;
    }

    unsigned h =   len + asso_values[static_cast<unsigned char>(str[1])] + asso_values[static_cast<unsigned char>(str[0])] + asso_values[static_cast<unsigned char>(str[len - 1])];

    if (h > MAX_HASH_VALUE) {
        return token_type::identifier;
    }
    const keyword_entry& entry = wordlist[h];
    __m128i mask = ::_mm_cmpgt_epi8(
        ::_mm_set1_epi8(len),
        ::_mm_setr_epi8(
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15));
    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry.keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)str);
    __m128i compared = ::_mm_xor_si128(entry_unmasked, identifier_unmasked);
    int comparison = ::_mm_test_all_zeros(mask, compared) != 1;
    if (comparison == 0) {
        comparison = entry.keyword[len];  // length check
    }
    if (comparison == 0) {
        return wordlist[h].type;
    } else {
        return token_type::identifier;
    }
}
}
