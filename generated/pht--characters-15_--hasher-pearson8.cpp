
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
constexpr std::uint32_t table_size = 256UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint8_t hash_seed[] = {
    0x7e, 0x8d, 0xaf, 0xef, 0xf4, 0x72, 0x15, 0x98, 0x69, 0x1d, 0xa0, 0x48, 0x03, 0xe5, 0xeb, 0x30, 0x55, 0x25, 0x04, 0x4c, 0x99, 0x1e, 0xc7, 0xee, 0xcd, 0x92, 0xc1, 0x88, 0x79, 0xaa, 0x09, 0xf2, 0xb2, 0x6d, 0x5d, 0xce, 0x16, 0x9b, 0xbb, 0x3a, 0xcc, 0x50, 0x17, 0xd7, 0xda, 0xfb, 0x2c, 0xf5, 0x37, 0x2e, 0x6b, 0x80, 0x57, 0xdb, 0xb1, 0x08, 0xa8, 0x86, 0xdd, 0x8c, 0x7f, 0x29, 0x34, 0x4b, 0x1a, 0x67, 0x90, 0x24, 0x77, 0x9e, 0xa3, 0x56, 0xd9, 0xd3, 0x8b, 0x02, 0x65, 0xd5, 0x07, 0x84, 0x11, 0xa2, 0xf7, 0x9a, 0xe9, 0xdf, 0x33, 0xbe, 0xa4, 0x75, 0xd1, 0xdc, 0xe4, 0xf1, 0x78, 0xd8, 0xcb, 0xac, 0x81, 0xc4, 0xe6, 0x58, 0x7d, 0x83, 0x32, 0xde, 0x5b, 0x8a, 0x4f, 0x05, 0x53, 0xfd, 0x93, 0x2d, 0x23, 0x74, 0x31, 0x9c, 0x70, 0x20, 0x18, 0x19, 0xe1, 0x39, 0xa9, 0x00, 0x45, 0x5e, 0x51, 0x36, 0xf9, 0xae, 0x8e, 0x6a, 0x0b, 0x06, 0x44, 0x52, 0xf6, 0xc6, 0x0a, 0xc8, 0x28, 0xb7, 0x26, 0xab, 0x1f, 0xed, 0xa7, 0x5c, 0x7a, 0x89, 0xad, 0xea, 0x63, 0x21, 0xa1, 0x5a, 0x42, 0x82, 0xba, 0xc2, 0xb8, 0x96, 0x40, 0x85, 0xb4, 0xbf, 0x22, 0xa5, 0x76, 0x27, 0x4d, 0xc9, 0x4a, 0xcf, 0xb6, 0x61, 0xf0, 0x5f, 0xc5, 0x41, 0x97, 0xd2, 0xbc, 0x1b, 0x2f, 0x14, 0x7b, 0x62, 0xf3, 0x94, 0x9d, 0x73, 0x10, 0xc0, 0x3d, 0x13, 0x49, 0x47, 0x3c, 0xfc, 0x4e, 0xb0, 0x91, 0x46, 0x0f, 0xec, 0x38, 0x0d, 0xe3, 0x3f, 0xfe, 0xb3, 0x64, 0x2a, 0xc3, 0x3b, 0x9f, 0xe8, 0xd4, 0xd0, 0x54, 0x12, 0xb5, 0x01, 0x35, 0x0e, 0xe7, 0xd6, 0xca, 0x87, 0xfa, 0xbd, 0xb9, 0x95, 0x0c, 0xff, 0x43, 0x3e, 0x59, 0x68, 0x6e, 0x6f, 0xe0, 0x1c, 0x2b, 0x6c, 0xa6, 0x71, 0x8f, 0x66, 0x7c, 0x60, 0xf8, 0xe2,
};


struct table_entry {

    const char keyword[max_keyword_size + 1];

    token_type type;
};

constexpr table_entry table[table_size] = {
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {"", token_type::identifier},
  {{'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {"", token_type::identifier},
  {{'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {{'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {"", token_type::identifier},
  {{'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {{'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {"", token_type::identifier},
  {{'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {{'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {"", token_type::identifier},
  {{'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {"", token_type::identifier},
  {{'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {"", token_type::identifier},
  {{'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {{'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {{'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {{'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {{'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {{'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {"", token_type::identifier},
  {{'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {"", token_type::identifier},
  {{'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {"", token_type::identifier},
  {{'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {{'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {{'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {"", token_type::identifier},
  {{'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {{'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {{'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {{'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {{'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {{'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {{'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {{'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {"", token_type::identifier},
  {{'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {{'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {"", token_type::identifier},
  {{'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {{'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {{'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {{'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
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
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {{'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},

};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {

    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    pearson_8_hasher hasher(hash_seed);
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


    if (std::memcmp(identifier, entry_keyword, size) != 0
        || !length_ok()) {
        return token_type::identifier;
    }

    return (token_type)result;
}
}
