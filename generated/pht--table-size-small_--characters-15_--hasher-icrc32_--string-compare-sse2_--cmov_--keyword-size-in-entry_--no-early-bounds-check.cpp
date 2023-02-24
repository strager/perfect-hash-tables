
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
constexpr std::uint32_t table_size = 262UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166179746UL;


struct table_entry {

    const char keyword[max_keyword_size + 0];

    std::uint8_t keyword_length;

    token_type type;
};

constexpr table_entry table[table_size] = {
  {"", 0, token_type::identifier},
  {{'g','l','o','b','a','l','_','_','_','_','_',}, 6, token_type::kw_global},
  {{'b','o','o','l','e','a','n','_','_','_','_',}, 7, token_type::kw_boolean},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'c','a','t','c','h','_','_','_','_','_','_',}, 5, token_type::kw_catch},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'n','e','v','e','r','_','_','_','_','_','_',}, 5, token_type::kw_never},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'e','l','s','e','_','_','_','_','_','_','_',}, 4, token_type::kw_else},
  {{'y','i','e','l','d','_','_','_','_','_','_',}, 5, token_type::kw_yield},
  {"", 0, token_type::identifier},
  {{'t','r','y','_','_','_','_','_','_','_','_',}, 3, token_type::kw_try},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'c','a','s','e','_','_','_','_','_','_','_',}, 4, token_type::kw_case},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'v','a','r','_','_','_','_','_','_','_','_',}, 3, token_type::kw_var},
  {"", 0, token_type::identifier},
  {{'s','y','m','b','o','l','_','_','_','_','_',}, 6, token_type::kw_symbol},
  {{'a','n','y','_','_','_','_','_','_','_','_',}, 3, token_type::kw_any},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'f','u','n','c','t','i','o','n','_','_','_',}, 8, token_type::kw_function},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'d','e','b','u','g','g','e','r','_','_','_',}, 8, token_type::kw_debugger},
  {{'f','r','o','m','_','_','_','_','_','_','_',}, 4, token_type::kw_from},
  {{'o','u','t','_','_','_','_','_','_','_','_',}, 3, token_type::kw_out},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'r','e','t','u','r','n','_','_','_','_','_',}, 6, token_type::kw_return},
  {{'t','r','u','e','_','_','_','_','_','_','_',}, 4, token_type::kw_true},
  {{'p','r','o','t','e','c','t','e','d','_','_',}, 9, token_type::kw_protected},
  {{'i','n','f','e','r','_','_','_','_','_','_',}, 5, token_type::kw_infer},
  {{'t','y','p','e','o','f','_','_','_','_','_',}, 6, token_type::kw_typeof},
  {{'f','o','r','_','_','_','_','_','_','_','_',}, 3, token_type::kw_for},
  {{'o','f','_','_','_','_','_','_','_','_','_',}, 2, token_type::kw_of},
  {{'d','o','_','_','_','_','_','_','_','_','_',}, 2, token_type::kw_do},
  {"", 0, token_type::identifier},
  {{'n','a','m','e','s','p','a','c','e','_','_',}, 9, token_type::kw_namespace},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'w','i','t','h','_','_','_','_','_','_','_',}, 4, token_type::kw_with},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'p','u','b','l','i','c','_','_','_','_','_',}, 6, token_type::kw_public},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'i','n','s','t','a','n','c','e','o','f','_',}, 10, token_type::kw_instanceof},
  {{'a','b','s','t','r','a','c','t','_','_','_',}, 8, token_type::kw_abstract},
  {{'m','o','d','u','l','e','_','_','_','_','_',}, 6, token_type::kw_module},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'n','u','l','l','_','_','_','_','_','_','_',}, 4, token_type::kw_null},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'d','e','l','e','t','e','_','_','_','_','_',}, 6, token_type::kw_delete},
  {{'e','x','p','o','r','t','_','_','_','_','_',}, 6, token_type::kw_export},
  {"", 0, token_type::identifier},
  {{'a','s','y','n','c','_','_','_','_','_','_',}, 5, token_type::kw_async},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'e','x','t','e','n','d','s','_','_','_','_',}, 7, token_type::kw_extends},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'o','v','e','r','r','i','d','e','_','_','_',}, 8, token_type::kw_override},
  {"", 0, token_type::identifier},
  {{'n','e','w','_','_','_','_','_','_','_','_',}, 3, token_type::kw_new},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'w','h','i','l','e','_','_','_','_','_','_',}, 5, token_type::kw_while},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'c','o','n','s','t','_','_','_','_','_','_',}, 5, token_type::kw_const},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'v','o','i','d','_','_','_','_','_','_','_',}, 4, token_type::kw_void},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'a','w','a','i','t','_','_','_','_','_','_',}, 5, token_type::kw_await},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'o','b','j','e','c','t','_','_','_','_','_',}, 6, token_type::kw_object},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'a','s','s','e','r','t','s','_','_','_','_',}, 7, token_type::kw_asserts},
  {"", 0, token_type::identifier},
  {{'r','e','a','d','o','n','l','y','_','_','_',}, 8, token_type::kw_readonly},
  {{'e','n','u','m','_','_','_','_','_','_','_',}, 4, token_type::kw_enum},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'p','r','i','v','a','t','e','_','_','_','_',}, 7, token_type::kw_private},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'s','t','r','i','n','g','_','_','_','_','_',}, 6, token_type::kw_string},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'i','n','t','r','i','n','s','i','c','_','_',}, 9, token_type::kw_intrinsic},
  {{'t','h','r','o','w','_','_','_','_','_','_',}, 5, token_type::kw_throw},
  {{'s','e','t','_','_','_','_','_','_','_','_',}, 3, token_type::kw_set},
  {"", 0, token_type::identifier},
  {{'r','e','q','u','i','r','e','_','_','_','_',}, 7, token_type::kw_require},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'u','n','d','e','f','i','n','e','d','_','_',}, 9, token_type::kw_undefined},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'c','o','n','t','i','n','u','e','_','_','_',}, 8, token_type::kw_continue},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'c','o','n','s','t','r','u','c','t','o','r',}, 11, token_type::kw_constructor},
  {{'s','u','p','e','r','_','_','_','_','_','_',}, 5, token_type::kw_super},
  {"", 0, token_type::identifier},
  {{'a','s','_','_','_','_','_','_','_','_','_',}, 2, token_type::kw_as},
  {{'i','f','_','_','_','_','_','_','_','_','_',}, 2, token_type::kw_if},
  {"", 0, token_type::identifier},
  {{'b','i','g','i','n','t','_','_','_','_','_',}, 6, token_type::kw_bigint},
  {{'i','n','t','e','r','f','a','c','e','_','_',}, 9, token_type::kw_interface},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'t','y','p','e','_','_','_','_','_','_','_',}, 4, token_type::kw_type},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'n','u','m','b','e','r','_','_','_','_','_',}, 6, token_type::kw_number},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'k','e','y','o','f','_','_','_','_','_','_',}, 5, token_type::kw_keyof},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'f','a','l','s','e','_','_','_','_','_','_',}, 5, token_type::kw_false},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'s','w','i','t','c','h','_','_','_','_','_',}, 6, token_type::kw_switch},
  {{'d','e','c','l','a','r','e','_','_','_','_',}, 7, token_type::kw_declare},
  {"", 0, token_type::identifier},
  {{'l','e','t','_','_','_','_','_','_','_','_',}, 3, token_type::kw_let},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'b','r','e','a','k','_','_','_','_','_','_',}, 5, token_type::kw_break},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'i','n','_','_','_','_','_','_','_','_','_',}, 2, token_type::kw_in},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'i','m','p','l','e','m','e','n','t','s','_',}, 10, token_type::kw_implements},
  {"", 0, token_type::identifier},
  {{'g','e','t','_','_','_','_','_','_','_','_',}, 3, token_type::kw_get},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'a','s','s','e','r','t','_','_','_','_','_',}, 6, token_type::kw_assert},
  {{'c','l','a','s','s','_','_','_','_','_','_',}, 5, token_type::kw_class},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'f','i','n','a','l','l','y','_','_','_','_',}, 7, token_type::kw_finally},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {{'s','t','a','t','i','c','_','_','_','_','_',}, 6, token_type::kw_static},
  {{'i','m','p','o','r','t','_','_','_','_','_',}, 6, token_type::kw_import},
  {{'t','h','i','s','_','_','_','_','_','_','_',}, 4, token_type::kw_this},
  {"", 0, token_type::identifier},
  {{'i','s','_','_','_','_','_','_','_','_','_',}, 2, token_type::kw_is},
  {"", 0, token_type::identifier},
  {{'u','n','k','n','o','w','n','_','_','_','_',}, 7, token_type::kw_unknown},
  {{'p','a','c','k','a','g','e','_','_','_','_',}, 7, token_type::kw_package},
  {"", 0, token_type::identifier},
  {{'u','n','i','q','u','e','_','_','_','_','_',}, 6, token_type::kw_unique},
  {{'d','e','f','a','u','l','t','_','_','_','_',}, 7, token_type::kw_default},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},
  {"", 0, token_type::identifier},

};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {

    intel_crc32_intrinsic_hasher hasher(hash_seed);
    hash_selected_characters(character_selection, hasher, identifier, size);
    std::uint32_t h = hasher.hash();
    std::uint32_t index = hash_to_index(h, table_size, sizeof(table_entry), hash_to_index_strategy::modulo);

    const table_entry& entry = table[index];

    auto length_ok = [&]() -> bool {

        return size == entry.keyword_length;

    };

    int result = (int)entry.type;

#if defined(__x86_64__)
    auto check_length_cmov = [&]() -> void {

        __asm__(
            "cmpq %[size], %[entry_keyword_length]\n"
            "cmovne %[token_type_identifier], %[result]\n"

            : [result]"+r"(result)

            : [entry_keyword_length]"r"((unsigned long)entry.keyword_length),
              [size]"r"((unsigned long)size),
              [token_type_identifier]"r"((int)token_type::identifier)

            : "cc"   // Clobbered by cmp.
        );

    };
#endif


    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry.keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)identifier);
    // Calculating the mask this way seems to be much much faster than '(1 << size) - 1'.
    std::uint32_t inv_mask = ~(std::uint32_t)0 << size;
    std::uint32_t mask = ~inv_mask;
    std::uint32_t equal_mask = ::_mm_movemask_epi8(::_mm_cmpeq_epi8(entry_unmasked, identifier_unmasked));
    std::uint32_t not_equal_mask = ~equal_mask;

    check_length_cmov();

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
