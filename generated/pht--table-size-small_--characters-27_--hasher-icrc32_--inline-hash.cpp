
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
constexpr character_selection_mask character_selection = 27U;
constexpr std::uint32_t table_size = 253UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166172010UL;


struct table_entry {

    std::uint32_t hash;

    const char keyword[max_keyword_size + 1];
    token_type type;
};

constexpr table_entry table[table_size] = {
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {762260415U, {'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {731380503U, {'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {768794397U, {'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {1139095824U, {'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {3990137139U, {'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {4080987669U, {'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1964116685U, {'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {2739465040U, {'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2926902119U, {'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {2863609364U, {'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {0U, "", token_type::identifier},
  {4187408843U, {'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3005514539U, {'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2164723184U, {'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2231946047U, {'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3872416776U, {'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {0U, "", token_type::identifier},
  {541215366U, {'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {3697427052U, {'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3480860070U, {'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3856418843U, {'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2074445480U, {'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3670591873U, {'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {2782172507U, {'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1251503283U, {'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {0U, "", token_type::identifier},
  {664412990U, {'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3336611897U, {'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {210328094U, {'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4174381938U, {'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {0U, "", token_type::identifier},
  {1989373246U, {'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3647571177U, {'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2317877562U, {'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
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
  {3460998365U, {'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {2589732399U, {'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {3589382205U, {'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {950049778U, {'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1098862361U, {'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1195009954U, {'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {3158499439U, {'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4192732577U, {'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {1517392174U, {'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1419872821U, {'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3766528531U, {'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2451195955U, {'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {0U, "", token_type::identifier},
  {3417299227U, {'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1137227306U, {'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1538467351U, {'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2897317704U, {'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {2356503134U, {'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1932064265U, {'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4225046785U, {'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {0U, "", token_type::identifier},
  {4097717706U, {'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {4016677759U, {'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {0U, "", token_type::identifier},
  {359880529U, {'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {2977825860U, {'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {0U, "", token_type::identifier},
  {943827049U, {'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {0U, "", token_type::identifier},
  {1808962575U, {'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {3949708414U, {'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {2223968344U, {'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4231994944U, {'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {319212569U, {'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2264159690U, {'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1444830579U, {'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {626587091U, {'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {0U, "", token_type::identifier},
  {3360177458U, {'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2046905062U, {'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3462944547U, {'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {4054097011U, {'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {1765261925U, {'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1418708095U, {'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1860838502U, {'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {2677686447U, {'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {0U, "", token_type::identifier},
  {412113U, {'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {672087901U, {'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1546573805U, {'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {1670352032U, {'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {0U, "", token_type::identifier},
  {1905669611U, {'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {0U, "", token_type::identifier},
  {2405799507U, {'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {2669558080U, {'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {3250898445U, {'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {741662159U, {'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {726977281U, {'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
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
