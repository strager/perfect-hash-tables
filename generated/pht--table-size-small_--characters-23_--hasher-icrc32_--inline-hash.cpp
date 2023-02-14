
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
constexpr std::uint32_t table_size = 246UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166172673UL;


struct table_entry {

    std::uint32_t hash;

    const char keyword[max_keyword_size + 1];
    token_type type;
};

constexpr table_entry table[table_size] = {
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {157494861U, {'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {753917926U, {'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3632782623U, {'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {3883440142U, {'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
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
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2022980043U, {'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {4219086988U, {'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2109303661U, {'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {1735799648U, {'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {928367379U, {'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {2624139106U, {'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {755803795U, {'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {760619984U, {'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1709981958U, {'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
  {2446710139U, {'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {0U, "", token_type::identifier},
  {2285726757U, {'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {0U, "", token_type::identifier},
  {1474746185U, {'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {4166325174U, {'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1656260489U, {'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3289348221U, {'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1096867812U, {'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1071208293U, {'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1764568400U, {'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2733417020U, {'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3170420529U, {'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {0U, "", token_type::identifier},
  {900395999U, {'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3445721348U, {'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {1845668961U, {'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2175700596U, {'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {437306913U, {'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {1126371118U, {'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1525266764U, {'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {0U, "", token_type::identifier},
  {1021776352U, {'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3821053406U, {'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {1384563627U, {'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1993265053U, {'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {0U, "", token_type::identifier},
  {161549055U, {'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1228356842U, {'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4150524428U, {'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {59007662U, {'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1761126685U, {'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {0U, "", token_type::identifier},
  {623141265U, {'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {0U, "", token_type::identifier},
  {2997186407U, {'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {0U, "", token_type::identifier},
  {3057425659U, {'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {526472126U, {'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4212126542U, {'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {0U, "", token_type::identifier},
  {829794562U, {'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {3784191095U, {'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {348065066U, {'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1171309727U, {'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {343215923U, {'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {49934478U, {'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {486190633U, {'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {0U, "", token_type::identifier},
  {1195833231U, {'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {0U, "", token_type::identifier},
  {2416140341U, {'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {764956362U, {'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1912082903U, {'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {1286434302U, {'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {4256617639U, {'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {2678579054U, {'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4214303922U, {'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {1163456695U, {'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {348208514U, {'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {3137368078U, {'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {0U, "", token_type::identifier},
  {299168418U, {'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {3315264457U, {'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {0U, "", token_type::identifier},
  {3075149271U, {'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {1486359502U, {'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {763076699U, {'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2738039244U, {'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3341327681U, {'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {2928871290U, {'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2285035666U, {'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {0U, "", token_type::identifier},
  {644434608U, {'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {1770991855U, {'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3004767466U, {'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3749983148U, {'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {2911130037U, {'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {1216322140U, {'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {0U, "", token_type::identifier},
  {2055492348U, {'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3644475973U, {'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},

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
