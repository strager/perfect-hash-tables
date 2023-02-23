
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

    std::uint32_t hash;

    const char keyword[max_keyword_size + 1];

    token_type type;
};

constexpr table_entry table[table_size] = {
  {0U, "", token_type::identifier},
  {1552259611U, {'g','l','o','b','a','l',0,'_','_','_','_','_',}, token_type::kw_global},
  {4059541972U, {'b','o','o','l','e','a','n',0,'_','_','_','_',}, token_type::kw_boolean},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3300263096U, {'c','a','t','c','h',0,'_','_','_','_','_','_',}, token_type::kw_catch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {406382186U, {'n','e','v','e','r',0,'_','_','_','_','_','_',}, token_type::kw_never},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2211488569U, {'e','l','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_else},
  {4137291274U, {'y','i','e','l','d',0,'_','_','_','_','_','_',}, token_type::kw_yield},
  {0U, "", token_type::identifier},
  {1611439404U, {'t','r','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_try},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1146862058U, {'c','a','s','e',0,'_','_','_','_','_','_','_',}, token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3612143991U, {'v','a','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_var},
  {0U, "", token_type::identifier},
  {1898300337U, {'s','y','m','b','o','l',0,'_','_','_','_','_',}, token_type::kw_symbol},
  {360252394U, {'a','n','y',0,'_','_','_','_','_','_','_','_',}, token_type::kw_any},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2039612399U, {'f','u','n','c','t','i','o','n',0,'_','_','_',}, token_type::kw_function},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1360766477U, {'d','e','b','u','g','g','e','r',0,'_','_','_',}, token_type::kw_debugger},
  {506631282U, {'f','r','o','m',0,'_','_','_','_','_','_','_',}, token_type::kw_from},
  {2897946417U, {'o','u','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_out},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1687106873U, {'r','e','t','u','r','n',0,'_','_','_','_','_',}, token_type::kw_return},
  {1199332566U, {'t','r','u','e',0,'_','_','_','_','_','_','_',}, token_type::kw_true},
  {4059045799U, {'p','r','o','t','e','c','t','e','d',0,'_','_',}, token_type::kw_protected},
  {3544686614U, {'i','n','f','e','r',0,'_','_','_','_','_','_',}, token_type::kw_infer},
  {63394627U, {'t','y','p','e','o','f',0,'_','_','_','_','_',}, token_type::kw_typeof},
  {588454418U, {'f','o','r',0,'_','_','_','_','_','_','_','_',}, token_type::kw_for},
  {1120284551U, {'o','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_of},
  {2599541006U, {'d','o',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_do},
  {0U, "", token_type::identifier},
  {733851846U, {'n','a','m','e','s','p','a','c','e',0,'_','_',}, token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {722043511U, {'w','i','t','h',0,'_','_','_','_','_','_','_',}, token_type::kw_with},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2186591553U, {'p','u','b','l','i','c',0,'_','_','_','_','_',}, token_type::kw_public},
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
  {3201434416U, {'i','n','s','t','a','n','c','e','o','f',0,'_',}, token_type::kw_instanceof},
  {3437424629U, {'a','b','s','t','r','a','c','t',0,'_','_','_',}, token_type::kw_abstract},
  {1191640016U, {'m','o','d','u','l','e',0,'_','_','_','_','_',}, token_type::kw_module},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1510652007U, {'n','u','l','l',0,'_','_','_','_','_','_','_',}, token_type::kw_null},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3119017087U, {'d','e','l','e','t','e',0,'_','_','_','_','_',}, token_type::kw_delete},
  {2621154994U, {'e','x','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_export},
  {0U, "", token_type::identifier},
  {497654166U, {'a','s','y','n','c',0,'_','_','_','_','_','_',}, token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {519791338U, {'e','x','t','e','n','d','s',0,'_','_','_','_',}, token_type::kw_extends},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2294794968U, {'o','v','e','r','r','i','d','e',0,'_','_','_',}, token_type::kw_override},
  {0U, "", token_type::identifier},
  {768209526U, {'n','e','w',0,'_','_','_','_','_','_','_','_',}, token_type::kw_new},
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
  {2477879011U, {'w','h','i','l','e',0,'_','_','_','_','_','_',}, token_type::kw_while},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1334719828U, {'c','o','n','s','t',0,'_','_','_','_','_','_',}, token_type::kw_const},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2504514202U, {'v','o','i','d',0,'_','_','_','_','_','_','_',}, token_type::kw_void},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1032738637U, {'a','w','a','i','t',0,'_','_','_','_','_','_',}, token_type::kw_await},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4276330081U, {'o','b','j','e','c','t',0,'_','_','_','_','_',}, token_type::kw_object},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2288213038U, {'a','s','s','e','r','t','s',0,'_','_','_','_',}, token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {1795552694U, {'r','e','a','d','o','n','l','y',0,'_','_','_',}, token_type::kw_readonly},
  {799668425U, {'e','n','u','m',0,'_','_','_','_','_','_','_',}, token_type::kw_enum},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {805107026U, {'p','r','i','v','a','t','e',0,'_','_','_','_',}, token_type::kw_private},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4173396060U, {'s','t','r','i','n','g',0,'_','_','_','_','_',}, token_type::kw_string},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3807135262U, {'i','n','t','r','i','n','s','i','c',0,'_','_',}, token_type::kw_intrinsic},
  {873593051U, {'t','h','r','o','w',0,'_','_','_','_','_','_',}, token_type::kw_throw},
  {2279491602U, {'s','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_set},
  {0U, "", token_type::identifier},
  {4245069314U, {'r','e','q','u','i','r','e',0,'_','_','_','_',}, token_type::kw_require},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2176476089U, {'u','n','d','e','f','i','n','e','d',0,'_','_',}, token_type::kw_undefined},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3295944714U, {'c','o','n','t','i','n','u','e',0,'_','_','_',}, token_type::kw_continue},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2239005017U, {'c','o','n','s','t','r','u','c','t','o','r',0,}, token_type::kw_constructor},
  {3520975472U, {'s','u','p','e','r',0,'_','_','_','_','_','_',}, token_type::kw_super},
  {0U, "", token_type::identifier},
  {3935196164U, {'a','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_as},
  {4014763207U, {'i','f',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_if},
  {0U, "", token_type::identifier},
  {3162743401U, {'b','i','g','i','n','t',0,'_','_','_','_','_',}, token_type::kw_bigint},
  {2115785376U, {'i','n','t','e','r','f','a','c','e',0,'_','_',}, token_type::kw_interface},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3358284747U, {'t','y','p','e',0,'_','_','_','_','_','_','_',}, token_type::kw_type},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1754962912U, {'n','u','m','b','e','r',0,'_','_','_','_','_',}, token_type::kw_number},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3553287687U, {'k','e','y','o','f',0,'_','_','_','_','_','_',}, token_type::kw_keyof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3795591841U, {'f','a','l','s','e',0,'_','_','_','_','_','_',}, token_type::kw_false},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2361474584U, {'s','w','i','t','c','h',0,'_','_','_','_','_',}, token_type::kw_switch},
  {3492232525U, {'d','e','c','l','a','r','e',0,'_','_','_','_',}, token_type::kw_declare},
  {0U, "", token_type::identifier},
  {2180915715U, {'l','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_let},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3077230819U, {'b','r','e','a','k',0,'_','_','_','_','_','_',}, token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1573128909U, {'i','n',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_in},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3882191504U, {'i','m','p','l','e','m','e','n','t','s',0,'_',}, token_type::kw_implements},
  {0U, "", token_type::identifier},
  {362334428U, {'g','e','t',0,'_','_','_','_','_','_','_','_',}, token_type::kw_get},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {895904247U, {'a','s','s','e','r','t',0,'_','_','_','_','_',}, token_type::kw_assert},
  {1902809548U, {'c','l','a','s','s',0,'_','_','_','_','_','_',}, token_type::kw_class},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1607266215U, {'f','i','n','a','l','l','y',0,'_','_','_','_',}, token_type::kw_finally},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1161274886U, {'s','t','a','t','i','c',0,'_','_','_','_','_',}, token_type::kw_static},
  {1516289865U, {'i','m','p','o','r','t',0,'_','_','_','_','_',}, token_type::kw_import},
  {2588163830U, {'t','h','i','s',0,'_','_','_','_','_','_','_',}, token_type::kw_this},
  {0U, "", token_type::identifier},
  {2178872410U, {'i','s',0,'_','_','_','_','_','_','_','_','_',}, token_type::kw_is},
  {0U, "", token_type::identifier},
  {4074277614U, {'u','n','k','n','o','w','n',0,'_','_','_','_',}, token_type::kw_unknown},
  {3193049523U, {'p','a','c','k','a','g','e',0,'_','_','_','_',}, token_type::kw_package},
  {0U, "", token_type::identifier},
  {1276383547U, {'u','n','i','q','u','e',0,'_','_','_','_','_',}, token_type::kw_unique},
  {4178319914U, {'d','e','f','a','u','l','t',0,'_','_','_','_',}, token_type::kw_default},
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

    auto length_ok = [&]() -> bool {

        return entry.keyword[size] == '\0';

    };

    int result = (int)entry.type;

#if defined(__x86_64__)
    auto check_length_cmov = [&]() -> void {

        __asm__(
            // If what should be the null terminator is not null, then
            // (size != strlen(entry.keyword)), so set result to
            // token_type::identifier.
            "cmpb $0, %[entry_keyword_at_size]\n"
            "cmovne %[token_type_identifier], %[result]\n"

            : [result]"+r"(result)

            : [entry_keyword_at_size]"m"(entry.keyword[size]),
              [token_type_identifier]"r"((int)token_type::identifier)

            : "cc"   // Clobbered by cmp.
        );

    };
#endif


    if (h != entry.hash) {
        return token_type::identifier;
    }

    if (std::memcmp(identifier, entry.keyword, size) != 0
        || !length_ok()) {
        return token_type::identifier;
    }

    return (token_type)result;
}
}
