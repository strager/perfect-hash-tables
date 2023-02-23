
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
constexpr std::uint32_t table_size = 512UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;
constexpr std::uint32_t hash_seed = 2166136600UL;


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
  {520952838U, {'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {0U, "", token_type::identifier},
  {528837128U, {'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {513267214U, {'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528898071U, {'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524735006U, {'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {0U, "", token_type::identifier},
  {528730144U, {'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {0U, "", token_type::identifier},
  {532894754U, {'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {544422953U, {'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517186094U, {'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
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
  {536690745U, {'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509286973U, {'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {520831552U, {'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517278789U, {'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {513191499U, {'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {0U, "", token_type::identifier},
  {536462413U, {'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
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
  {513221223U, {'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
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
  {528959609U, {'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517095549U, {'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
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
  {528731272U, {'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {520938126U, {'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {0U, "", token_type::identifier},
  {524827280U, {'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524827284U, {'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {0U, "", token_type::identifier},
  {517034134U, {'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {528654487U, {'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536614556U, {'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
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
  {520846507U, {'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {0U, "", token_type::identifier},
  {528730797U, {'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
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
  {532802238U, {'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
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
  {524857547U, {'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {520891598U, {'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517049557U, {'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
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
  {521029860U, {'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {509271781U, {'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
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
  {513343225U, {'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
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
  {528989449U, {'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
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
  {517049620U, {'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {532893978U, {'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {520938781U, {'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {0U, "", token_type::identifier},
  {544255263U, {'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {528639776U, {'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {0U, "", token_type::identifier},
  {509134114U, {'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517034790U, {'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517247276U, {'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509440303U, {'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
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
  {521029444U, {'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528730441U, {'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528791378U, {'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528761179U, {'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
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
  {532741479U, {'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {513221997U, {'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {520998769U, {'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {517124978U, {'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
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
  {513221521U, {'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536569755U, {'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
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
  {517079467U, {'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {509210540U, {'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {0U, "", token_type::identifier},
  {532802478U, {'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {524857784U, {'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
  {520846265U, {'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {528806330U, {'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {528806331U, {'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {536707009U, {'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {0U, "", token_type::identifier},
  {524780995U, {'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517095369U, {'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528837068U, {'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
  {0U, "", token_type::identifier},
  {525055438U, {'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {0U, "", token_type::identifier},
  {517186512U, {'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
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
  {524978653U, {'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {0U, "", token_type::identifier},
  {536798687U, {'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {517033955U, {'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {0U, "", token_type::identifier},
  {528792037U, {'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {524734950U, {'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {525025768U, {'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {0U, "", token_type::identifier},
  {517187050U, {'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {528990703U, {'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {509379064U, {'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {540518905U, {'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {509379066U, {'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {509379067U, {'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
  {0U, "", token_type::identifier},
  {509379069U, {'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
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
    int result = (int)entry.type;

    if (h != entry.hash) {
        return token_type::identifier;
    }

    int comparison = std::memcmp(identifier, entry.keyword, size);
    if (comparison == 0) {
        comparison = entry.keyword[size];  // length check
    }

    if (comparison != 0) {
        result = (int)token_type::identifier;
    }

    return (token_type)result;
}
}
