
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
constexpr character_selection_mask character_selection = 23U;
constexpr std::uint32_t table_size = 253UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166150074UL;


struct table_entry {

    const char keyword[max_keyword_size + 1];

    token_type type;
};

constexpr table_entry table[table_size] = {
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {"", token_type::identifier},
  {{'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {{'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {{'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {{'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {{'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {"", token_type::identifier},
  {{'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {{'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {{'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {{'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {{'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {{'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {"", token_type::identifier},
  {{'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {{'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {{'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
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
  {{'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {{'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {{'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {{'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {"", token_type::identifier},
  {{'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {{'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {{'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {{'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {{'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {"", token_type::identifier},
  {{'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {{'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {{'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {{'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {{'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {"", token_type::identifier},
  {{'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {"", token_type::identifier},
  {{'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {{'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {{'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {{'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {"", token_type::identifier},
  {{'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {"", token_type::identifier},
  {{'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {"", token_type::identifier},
  {{'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
  {{'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {"", token_type::identifier},
  {{'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {{'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {{'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {{'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {{'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},

};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {

    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    aes_intrinsic_hasher hasher(hash_seed);
    hash_selected_characters(character_selection, hasher, identifier, size);
    std::uint32_t h = hasher.hash();
    std::uint32_t index = hash_to_index(h, table_size, sizeof(table_entry), hash_to_index_strategy::modulo);

    const table_entry& entry = table[index];
    const char* entry_keyword = table[index].keyword;

    auto length_ok = [&]() -> bool {

        return entry.keyword[size] == '\0';

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


    if (std::memcmp(identifier, entry_keyword, size) != 0
        || !length_ok()) {
        return token_type::identifier;
    }

    return (token_type)result;
}
}
