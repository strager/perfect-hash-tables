
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
constexpr std::uint32_t table_size = 512UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166143520UL;


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
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524687878U, {'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509209102U, {'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {0U, "", token_type::identifier},
  {532801040U, {'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {532801041U, {'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528927252U, {'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {0U, "", token_type::identifier},
  {532892182U, {'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {516925465U, {'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {536598042U, {'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528988192U, {'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {528988193U, {'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536537638U, {'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524992043U, {'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524992046U, {'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
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
  {528729657U, {'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {0U, "", token_type::identifier},
  {528637499U, {'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528820800U, {'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
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
  {517017681U, {'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
  {517017682U, {'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {517017683U, {'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517108822U, {'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524885595U, {'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
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
  {513295974U, {'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528759917U, {'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {521088110U, {'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {521088111U, {'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {0U, "", token_type::identifier},
  {517276273U, {'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517184120U, {'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
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
  {0U, "", token_type::identifier},
  {544528026U, {'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
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
  {520966826U, {'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {521088174U, {'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
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
  {524733627U, {'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528912062U, {'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517031619U, {'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
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
  {0U, "", token_type::identifier},
  {517200085U, {'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {513296095U, {'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509071081U, {'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
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
  {509118198U, {'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {509118199U, {'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {0U, "", token_type::identifier},
  {509118201U, {'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {0U, "", token_type::identifier},
  {513128699U, {'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
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
  {528759562U, {'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524993296U, {'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {0U, "", token_type::identifier},
  {520936210U, {'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
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
  {513128225U, {'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528789288U, {'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536705330U, {'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {521073461U, {'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509117246U, {'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {520966469U, {'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {516971338U, {'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536445780U, {'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {0U, "", token_type::identifier},
  {520951638U, {'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
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
  {528896870U, {'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {0U, "", token_type::identifier},
  {528637800U, {'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528636784U, {'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {528636785U, {'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {528636786U, {'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509209463U, {'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
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
  {517184898U, {'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {517154691U, {'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528896392U, {'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
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
  {524992403U, {'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524840854U, {'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {0U, "", token_type::identifier},
  {524886424U, {'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
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
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509163949U, {'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {0U, "", token_type::identifier},
  {509071791U, {'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536704434U, {'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {544359859U, {'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
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
  {521012163U, {'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {0U, "", token_type::identifier},
  {516924869U, {'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {0U, "", token_type::identifier},
  {528896455U, {'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {0U, "", token_type::identifier},
  {540441033U, {'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
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
  {513173975U, {'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {521088472U, {'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524839900U, {'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {532786149U, {'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {532786150U, {'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536399855U, {'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {517169648U, {'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {513189879U, {'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
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

    lehmer_128_hasher hasher(hash_seed);
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
