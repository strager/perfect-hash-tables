
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <nmmintrin.h>

namespace pht {
namespace {
constexpr character_selection_mask character_selection = 15U;
constexpr std::uint32_t table_size = 262UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166179746UL;


struct table_entry {

    const char keyword[max_keyword_size + 0];
    token_type type;
};

constexpr table_entry table[table_size] = {
  {"", token_type::identifier},
  {{'g','l','o','b','a','l','_','_','_','_','_',}, token_type::kw_global},
  {{'b','o','o','l','e','a','n','_','_','_','_',}, token_type::kw_boolean},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','a','t','c','h','_','_','_','_','_','_',}, token_type::kw_catch},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','e','v','e','r','_','_','_','_','_','_',}, token_type::kw_never},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'e','l','s','e','_','_','_','_','_','_','_',}, token_type::kw_else},
  {{'y','i','e','l','d','_','_','_','_','_','_',}, token_type::kw_yield},
  {"", token_type::identifier},
  {{'t','r','y','_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','a','s','e','_','_','_','_','_','_','_',}, token_type::kw_case},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'v','a','r','_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {"", token_type::identifier},
  {{'s','y','m','b','o','l','_','_','_','_','_',}, token_type::kw_symbol},
  {{'a','n','y','_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','u','n','c','t','i','o','n','_','_','_',}, token_type::kw_function},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','b','u','g','g','e','r','_','_','_',}, token_type::kw_debugger},
  {{'f','r','o','m','_','_','_','_','_','_','_',}, token_type::kw_from},
  {{'o','u','t','_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'r','e','t','u','r','n','_','_','_','_','_',}, token_type::kw_return},
  {{'t','r','u','e','_','_','_','_','_','_','_',}, token_type::kw_true},
  {{'p','r','o','t','e','c','t','e','d','_','_',}, token_type::kw_protected},
  {{'i','n','f','e','r','_','_','_','_','_','_',}, token_type::kw_infer},
  {{'t','y','p','e','o','f','_','_','_','_','_',}, token_type::kw_typeof},
  {{'f','o','r','_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {{'o','f','_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {{'d','o','_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {"", token_type::identifier},
  {{'n','a','m','e','s','p','a','c','e','_','_',}, token_type::kw_namespace},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'w','i','t','h','_','_','_','_','_','_','_',}, token_type::kw_with},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','u','b','l','i','c','_','_','_','_','_',}, token_type::kw_public},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','s','t','a','n','c','e','o','f','_',}, token_type::kw_instanceof},
  {{'a','b','s','t','r','a','c','t','_','_','_',}, token_type::kw_abstract},
  {{'m','o','d','u','l','e','_','_','_','_','_',}, token_type::kw_module},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','l','l','_','_','_','_','_','_','_',}, token_type::kw_null},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','l','e','t','e','_','_','_','_','_',}, token_type::kw_delete},
  {{'e','x','p','o','r','t','_','_','_','_','_',}, token_type::kw_export},
  {"", token_type::identifier},
  {{'a','s','y','n','c','_','_','_','_','_','_',}, token_type::kw_async},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'e','x','t','e','n','d','s','_','_','_','_',}, token_type::kw_extends},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','v','e','r','r','i','d','e','_','_','_',}, token_type::kw_override},
  {"", token_type::identifier},
  {{'n','e','w','_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'w','h','i','l','e','_','_','_','_','_','_',}, token_type::kw_while},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','o','n','s','t','_','_','_','_','_','_',}, token_type::kw_const},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'v','o','i','d','_','_','_','_','_','_','_',}, token_type::kw_void},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','w','a','i','t','_','_','_','_','_','_',}, token_type::kw_await},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','b','j','e','c','t','_','_','_','_','_',}, token_type::kw_object},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t','s','_','_','_','_',}, token_type::kw_asserts},
  {"", token_type::identifier},
  {{'r','e','a','d','o','n','l','y','_','_','_',}, token_type::kw_readonly},
  {{'e','n','u','m','_','_','_','_','_','_','_',}, token_type::kw_enum},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','r','i','v','a','t','e','_','_','_','_',}, token_type::kw_private},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','r','i','n','g','_','_','_','_','_',}, token_type::kw_string},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','t','r','i','n','s','i','c','_','_',}, token_type::kw_intrinsic},
  {{'t','h','r','o','w','_','_','_','_','_','_',}, token_type::kw_throw},
  {{'s','e','t','_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {"", token_type::identifier},
  {{'r','e','q','u','i','r','e','_','_','_','_',}, token_type::kw_require},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'u','n','d','e','f','i','n','e','d','_','_',}, token_type::kw_undefined},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','o','n','t','i','n','u','e','_','_','_',}, token_type::kw_continue},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','o','n','s','t','r','u','c','t','o','r',}, token_type::kw_constructor},
  {{'s','u','p','e','r','_','_','_','_','_','_',}, token_type::kw_super},
  {"", token_type::identifier},
  {{'a','s','_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {{'i','f','_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {"", token_type::identifier},
  {{'b','i','g','i','n','t','_','_','_','_','_',}, token_type::kw_bigint},
  {{'i','n','t','e','r','f','a','c','e','_','_',}, token_type::kw_interface},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'t','y','p','e','_','_','_','_','_','_','_',}, token_type::kw_type},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','m','b','e','r','_','_','_','_','_',}, token_type::kw_number},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'k','e','y','o','f','_','_','_','_','_','_',}, token_type::kw_keyof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','a','l','s','e','_','_','_','_','_','_',}, token_type::kw_false},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','w','i','t','c','h','_','_','_','_','_',}, token_type::kw_switch},
  {{'d','e','c','l','a','r','e','_','_','_','_',}, token_type::kw_declare},
  {"", token_type::identifier},
  {{'l','e','t','_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'b','r','e','a','k','_','_','_','_','_','_',}, token_type::kw_break},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','m','p','l','e','m','e','n','t','s','_',}, token_type::kw_implements},
  {"", token_type::identifier},
  {{'g','e','t','_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t','_','_','_','_','_',}, token_type::kw_assert},
  {{'c','l','a','s','s','_','_','_','_','_','_',}, token_type::kw_class},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','i','n','a','l','l','y','_','_','_','_',}, token_type::kw_finally},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','a','t','i','c','_','_','_','_','_',}, token_type::kw_static},
  {{'i','m','p','o','r','t','_','_','_','_','_',}, token_type::kw_import},
  {{'t','h','i','s','_','_','_','_','_','_','_',}, token_type::kw_this},
  {"", token_type::identifier},
  {{'i','s','_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {"", token_type::identifier},
  {{'u','n','k','n','o','w','n','_','_','_','_',}, token_type::kw_unknown},
  {{'p','a','c','k','a','g','e','_','_','_','_',}, token_type::kw_package},
  {"", token_type::identifier},
  {{'u','n','i','q','u','e','_','_','_','_','_',}, token_type::kw_unique},
  {{'d','e','f','a','u','l','t','_','_','_','_',}, token_type::kw_default},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},

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

    int comparison = _mm_cmpestri(
        ::_mm_loadu_si32(identifier),
        size,
        ::_mm_loadu_si32(entry.keyword),
        size,
        _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_LEAST_SIGNIFICANT);

    if (comparison == 0) {
        return entry.type;
    } else {
        return token_type::identifier;
    }

}
}
