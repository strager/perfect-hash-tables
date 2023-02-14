
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
constexpr character_selection_mask character_selection = 29U;
constexpr std::uint32_t table_size = 255UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166152985UL;


struct table_entry {

    std::uint32_t hash;

    const char keyword[max_keyword_size + 1];
    token_type type;
};

constexpr table_entry table[table_size] = {
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2073998132U, {'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3061379300U, {'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {0U, "", token_type::identifier},
  {2364803452U, {'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {983078303U, {'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {3086927754U, {'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1102750828U, {'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {941749439U, {'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1332256447U, {'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {351303325U, {'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {851092877U, {'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {849424162U, {'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {4218724373U, {'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {0U, "", token_type::identifier},
  {1928894500U, {'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {3375594671U, {'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
  {1951828182U, {'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3493053541U, {'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {0U, "", token_type::identifier},
  {2011704483U, {'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3071669106U, {'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {392345861U, {'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {328029252U, {'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {244858966U, {'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {0U, "", token_type::identifier},
  {1723942863U, {'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3471511928U, {'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {2790799884U, {'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {0U, "", token_type::identifier},
  {1387759796U, {'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {3754817187U, {'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {0U, "", token_type::identifier},
  {270494639U, {'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3455954897U, {'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {0U, "", token_type::identifier},
  {2049352459U, {'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1700084318U, {'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3404605561U, {'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {0U, "", token_type::identifier},
  {2339055933U, {'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1650093366U, {'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {1769246197U, {'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {0U, "", token_type::identifier},
  {1221387879U, {'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2881149478U, {'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4039937320U, {'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {0U, "", token_type::identifier},
  {2046372567U, {'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {4122331648U, {'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {3181525470U, {'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {0U, "", token_type::identifier},
  {403511357U, {'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {771852993U, {'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3268154586U, {'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2832472812U, {'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {2003588428U, {'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {436969670U, {'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {0U, "", token_type::identifier},
  {37409917U, {'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3947741594U, {'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {2465072145U, {'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3769245426U, {'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {0U, "", token_type::identifier},
  {157926758U, {'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4030190756U, {'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {4088336622U, {'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {275348908U, {'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3400960157U, {'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {639253295U, {'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3025976289U, {'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {926069908U, {'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {970053074U, {'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {505424719U, {'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {1516413020U, {'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2438052894U, {'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3245312009U, {'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {171062613U, {'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {0U, "", token_type::identifier},
  {1532423975U, {'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1462620542U, {'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {0U, "", token_type::identifier},
  {1384455904U, {'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3560294408U, {'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {0U, "", token_type::identifier},
  {3392737990U, {'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2677838610U, {'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {540692798U, {'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {1073392644U, {'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3278059409U, {'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {0U, "", token_type::identifier},
  {1365325576U, {'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {2357904662U, {'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {2957693478U, {'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {1881514939U, {'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {2529168530U, {'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {641255633U, {'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3322180034U, {'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},

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

    if (h != entry.hash) {
        return token_type::identifier;
    }

    int comparison = std::memcmp(identifier, entry.keyword, size);
    if (comparison == 0) {
        comparison = entry.keyword[size];  // length check
    }

    if (comparison == 0) {
        return entry.type;
    } else {
        return token_type::identifier;
    }

}
}
