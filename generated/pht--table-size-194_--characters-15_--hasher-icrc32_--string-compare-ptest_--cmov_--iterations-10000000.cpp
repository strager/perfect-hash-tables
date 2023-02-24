
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#if defined(__x86_64__)
#include <nmmintrin.h>
#endif
#if defined(__ARM_NEON)
#include <arm_neon.h>
#endif

namespace pht {
namespace {
constexpr character_selection_mask character_selection = 15U;
constexpr std::uint32_t table_size = 194UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2175834749UL;


struct table_entry {

    const char keyword[max_keyword_size + 1];

    token_type type;
};

constexpr table_entry table[table_size] = {
  {"", token_type::identifier},
  {{'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {"", token_type::identifier},
  {{'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {"", token_type::identifier},
  {{'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {"", token_type::identifier},
  {{'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {{'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {"", token_type::identifier},
  {{'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {{'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {{'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
  {{'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {{'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {"", token_type::identifier},
  {{'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {"", token_type::identifier},
  {{'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {{'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {"", token_type::identifier},
  {{'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {{'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {{'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {{'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {"", token_type::identifier},
  {{'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {"", token_type::identifier},
  {{'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {"", token_type::identifier},
  {{'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {"", token_type::identifier},
  {{'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {"", token_type::identifier},
  {{'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {{'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {{'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {"", token_type::identifier},
  {{'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {{'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
  {"", token_type::identifier},
  {{'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {{'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {{'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {"", token_type::identifier},
  {{'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {{'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {{'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {{'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {"", token_type::identifier},
  {{'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {{'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {"", token_type::identifier},
  {{'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {{'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {"", token_type::identifier},
  {{'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {{'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {{'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {{'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {"", token_type::identifier},
  {{'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {{'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {{'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {{'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {{'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {"", token_type::identifier},
  {{'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {"", token_type::identifier},
  {{'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {{'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {"", token_type::identifier},
  {{'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {"", token_type::identifier},
  {{'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},

};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {

    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    intel_crc32_intrinsic_hasher hasher(hash_seed);
    hash_selected_characters(character_selection, hasher, identifier, size);
    std::uint32_t h = hasher.hash();
    std::uint32_t index = hash_to_index(h, table_size, sizeof(table_entry), hash_to_index_strategy::modulo);

    const table_entry& entry = table[index];

    const char* entry_keyword = table[index].keyword;

    auto length_ok = [&]() -> bool {

        return entry_keyword[size] == '\0';

    };

    int result = (int)entry.type;

#if defined(__x86_64__)
    auto check_length_cmov = [&]() -> void {

        __asm__(
            // If what should be the null terminator is not null, then
            // (size != strlen(entry_keyword)), so set result to
            // token_type::identifier.
            "cmpb $0, %[entry_keyword_at_size]\n"
            "cmovne %[token_type_identifier], %[result]\n"

            : [result]"+r"(result)

            : [entry_keyword_at_size]"m"(entry_keyword[size]),
              [token_type_identifier]"r"((int)token_type::identifier)

            : "cc"   // Clobbered by cmp.
        );

    };
#endif


    __m128i mask = ::_mm_cmpgt_epi8(
        ::_mm_set1_epi8(size),
        ::_mm_setr_epi8(
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15));
    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry_keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)identifier);
    __m128i compared = ::_mm_xor_si128(entry_unmasked, identifier_unmasked);

    check_length_cmov();

    __asm__(
        // Compare the entry_keyword and identifier strings.
        "ptest %[compared], %[mask]\n"
        "cmovne %[token_type_identifier], %[result]\n"

        : [result]"+r"(result)

        : [compared]"x"(compared),
          [mask]"x"(mask),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by ptest.
    );

    return (token_type)result;
}
}
