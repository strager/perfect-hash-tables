
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
constexpr std::uint32_t table_size = 512UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166166546UL;


struct table_entry {

    std::uint16_t keyword_index;  // Index into string_table.

    std::uint8_t keyword_length;

    token_type type;
};

constexpr char string_table[] = {
'y','i','e','l','d','w','i','t','h','w','h','i','l','e','v','o','i','d','v','a','r','u','n','k','n','o','w','n','u','n','i','q','u','e','u','n','d','e','f','i','n','e','d','t','y','p','e','o','f','t','r','y','t','r','u','e','t','h','r','o','w','t','h','i','s','s','y','m','b','o','l','s','w','i','t','c','h','s','u','p','e','r','s','t','r','i','n','g','s','t','a','t','i','c','s','e','t','r','e','t','u','r','n','r','e','q','u','i','r','e','r','e','a','d','o','n','l','y','p','u','b','l','i','c','p','r','o','t','e','c','t','e','d','p','r','i','v','a','t','e','p','a','c','k','a','g','e','o','v','e','r','r','i','d','e','o','u','t','o','b','j','e','c','t','n','u','m','b','e','r','n','u','l','l','n','e','w','n','e','v','e','r','n','a','m','e','s','p','a','c','e','m','o','d','u','l','e','l','e','t','k','e','y','o','f','i','n','t','r','i','n','s','i','c','i','n','t','e','r','f','a','c','e','i','n','s','t','a','n','c','e','o','f','i','n','f','e','r','i','m','p','o','r','t','i','m','p','l','e','m','e','n','t','s','i','f','g','l','o','b','a','l','g','e','t','f','u','n','c','t','i','o','n','f','r','o','m','f','o','r','f','i','n','a','l','l','y','f','a','l','s','e','e','x','t','e','n','d','s','e','x','p','o','r','t','e','n','u','m','e','l','s','e','d','e','l','e','t','e','d','e','f','a','u','l','t','d','e','c','l','a','r','e','d','e','b','u','g','g','e','r','c','o','n','t','i','n','u','e','c','o','n','s','t','r','u','c','t','o','r','c','l','a','s','s','c','a','t','c','h','c','a','s','e','b','r','e','a','k','b','o','o','l','e','a','n','b','i','g','i','n','t','a','w','a','i','t','a','s','y','n','c','a','s','s','e','r','t','s','a','n','y','a','b','s','t','r','a','c','t',
};

constexpr table_entry table[table_size] = {
  {305, 4, token_type::kw_enum},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {47, 2, token_type::kw_of},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {63, 2, token_type::kw_is},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {28, 6, token_type::kw_unique},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {402, 6, token_type::kw_assert},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {170, 4, token_type::kw_null},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {5, 4, token_type::kw_with},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {280, 7, token_type::kw_finally},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {238, 6, token_type::kw_import},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {341, 8, token_type::kw_continue},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {370, 4, token_type::kw_case},
  {287, 5, token_type::kw_false},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {65, 6, token_type::kw_symbol},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {191, 6, token_type::kw_module},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {124, 9, token_type::kw_protected},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {174, 3, token_type::kw_new},
  {0, 0, token_type::identifier},
  {94, 3, token_type::kw_set},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {262, 3, token_type::kw_get},
  {0, 0, token_type::identifier},
  {197, 3, token_type::kw_let},
  {0, 0, token_type::identifier},
  {39, 2, token_type::kw_in},
  {0, 0, token_type::identifier},
  {360, 5, token_type::kw_class},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {97, 6, token_type::kw_return},
  {362, 2, token_type::kw_as},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {333, 8, token_type::kw_debugger},
  {0, 0, token_type::identifier},
  {177, 5, token_type::kw_never},
  {374, 5, token_type::kw_break},
  {214, 9, token_type::kw_interface},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {43, 6, token_type::kw_typeof},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {9, 5, token_type::kw_while},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {113, 2, token_type::kw_do},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {409, 3, token_type::kw_any},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {233, 5, token_type::kw_infer},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {256, 6, token_type::kw_global},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {402, 7, token_type::kw_asserts},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {21, 7, token_type::kw_unknown},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {273, 4, token_type::kw_from},
  {0, 5, token_type::kw_yield},
  {77, 5, token_type::kw_super},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {43, 4, token_type::kw_type},
  {164, 6, token_type::kw_number},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {61, 4, token_type::kw_this},
  {0, 0, token_type::identifier},
  {88, 6, token_type::kw_static},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {103, 7, token_type::kw_require},
  {244, 10, token_type::kw_implements},
  {254, 2, token_type::kw_if},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {326, 7, token_type::kw_declare},
  {133, 7, token_type::kw_private},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {205, 9, token_type::kw_intrinsic},
  {397, 5, token_type::kw_async},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {392, 5, token_type::kw_await},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {309, 4, token_type::kw_else},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {292, 7, token_type::kw_extends},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {71, 6, token_type::kw_switch},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {14, 4, token_type::kw_void},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {412, 8, token_type::kw_abstract},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {158, 6, token_type::kw_object},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {118, 6, token_type::kw_public},
  {34, 9, token_type::kw_undefined},
  {0, 0, token_type::identifier},
  {140, 7, token_type::kw_package},
  {349, 5, token_type::kw_const},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {147, 8, token_type::kw_override},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {82, 6, token_type::kw_string},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {299, 6, token_type::kw_export},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {365, 5, token_type::kw_catch},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {379, 7, token_type::kw_boolean},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {52, 4, token_type::kw_true},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {110, 8, token_type::kw_readonly},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {182, 9, token_type::kw_namespace},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {386, 6, token_type::kw_bigint},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {349, 11, token_type::kw_constructor},
  {277, 3, token_type::kw_for},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {313, 6, token_type::kw_delete},
  {319, 7, token_type::kw_default},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {200, 5, token_type::kw_keyof},
  {0, 0, token_type::identifier},
  {265, 8, token_type::kw_function},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {56, 5, token_type::kw_throw},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {223, 10, token_type::kw_instanceof},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {155, 3, token_type::kw_out},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {49, 3, token_type::kw_try},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {0, 0, token_type::identifier},
  {18, 3, token_type::kw_var},
  {0, 0, token_type::identifier},

};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {

    lehmer_128_hasher hasher(hash_seed);
    hash_selected_characters(character_selection, hasher, identifier, size);
    std::uint32_t h = hasher.hash();
    std::uint32_t index = hash_to_index(h, table_size, sizeof(table_entry), hash_to_index_strategy::modulo);

    const table_entry& entry = table[index];

    const char* entry_keyword = &string_table[table[index].keyword_index];

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
