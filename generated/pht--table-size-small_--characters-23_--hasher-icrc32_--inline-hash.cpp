
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

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
  {157494861U, "continue", token_type::kw_continue},
  {753917926U, "this", token_type::kw_this},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3632782623U, "delete", token_type::kw_delete},
  {3883440142U, "void", token_type::kw_void},
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
  {2022980043U, "string", token_type::kw_string},
  {4219086988U, "boolean", token_type::kw_boolean},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2109303661U, "object", token_type::kw_object},
  {1735799648U, "super", token_type::kw_super},
  {928367379U, "let", token_type::kw_let},
  {2624139106U, "return", token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {755803795U, "function", token_type::kw_function},
  {760619984U, "package", token_type::kw_package},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1709981958U, "infer", token_type::kw_infer},
  {2446710139U, "finally", token_type::kw_finally},
  {0U, "", token_type::identifier},
  {2285726757U, "default", token_type::kw_default},
  {0U, "", token_type::identifier},
  {1474746185U, "do", token_type::kw_do},
  {4166325174U, "unknown", token_type::kw_unknown},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1656260489U, "in", token_type::kw_in},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3289348221U, "case", token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1096867812U, "interface", token_type::kw_interface},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1071208293U, "bigint", token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1764568400U, "break", token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2733417020U, "null", token_type::kw_null},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3170420529U, "private", token_type::kw_private},
  {0U, "", token_type::identifier},
  {900395999U, "class", token_type::kw_class},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3445721348U, "number", token_type::kw_number},
  {1845668961U, "require", token_type::kw_require},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2175700596U, "instanceof", token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {437306913U, "out", token_type::kw_out},
  {1126371118U, "declare", token_type::kw_declare},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1525266764U, "if", token_type::kw_if},
  {0U, "", token_type::identifier},
  {1021776352U, "protected", token_type::kw_protected},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3821053406U, "abstract", token_type::kw_abstract},
  {1384563627U, "switch", token_type::kw_switch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1993265053U, "yield", token_type::kw_yield},
  {0U, "", token_type::identifier},
  {161549055U, "catch", token_type::kw_catch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1228356842U, "async", token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4150524428U, "of", token_type::kw_of},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {59007662U, "else", token_type::kw_else},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1761126685U, "symbol", token_type::kw_symbol},
  {0U, "", token_type::identifier},
  {623141265U, "enum", token_type::kw_enum},
  {0U, "", token_type::identifier},
  {2997186407U, "keyof", token_type::kw_keyof},
  {0U, "", token_type::identifier},
  {3057425659U, "assert", token_type::kw_assert},
  {526472126U, "export", token_type::kw_export},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4212126542U, "readonly", token_type::kw_readonly},
  {0U, "", token_type::identifier},
  {829794562U, "set", token_type::kw_set},
  {3784191095U, "while", token_type::kw_while},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {348065066U, "from", token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1171309727U, "override", token_type::kw_override},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {343215923U, "with", token_type::kw_with},
  {49934478U, "static", token_type::kw_static},
  {486190633U, "any", token_type::kw_any},
  {0U, "", token_type::identifier},
  {1195833231U, "var", token_type::kw_var},
  {0U, "", token_type::identifier},
  {2416140341U, "false", token_type::kw_false},
  {764956362U, "asserts", token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1912082903U, "typeof", token_type::kw_typeof},
  {1286434302U, "undefined", token_type::kw_undefined},
  {4256617639U, "implements", token_type::kw_implements},
  {2678579054U, "constructor", token_type::kw_constructor},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4214303922U, "intrinsic", token_type::kw_intrinsic},
  {1163456695U, "global", token_type::kw_global},
  {348208514U, "namespace", token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {3137368078U, "extends", token_type::kw_extends},
  {0U, "", token_type::identifier},
  {299168418U, "is", token_type::kw_is},
  {3315264457U, "public", token_type::kw_public},
  {0U, "", token_type::identifier},
  {3075149271U, "throw", token_type::kw_throw},
  {1486359502U, "debugger", token_type::kw_debugger},
  {763076699U, "unique", token_type::kw_unique},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2738039244U, "get", token_type::kw_get},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3341327681U, "true", token_type::kw_true},
  {2928871290U, "never", token_type::kw_never},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2285035666U, "new", token_type::kw_new},
  {0U, "", token_type::identifier},
  {644434608U, "module", token_type::kw_module},
  {1770991855U, "try", token_type::kw_try},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3004767466U, "for", token_type::kw_for},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3749983148U, "const", token_type::kw_const},
  {2911130037U, "await", token_type::kw_await},
  {1216322140U, "type", token_type::kw_type},
  {0U, "", token_type::identifier},
  {2055492348U, "as", token_type::kw_as},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3644475973U, "import", token_type::kw_import},
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
    std::uint32_t index = h % table_size;

    const table_entry& entry = table[index];

    if (h != entry.hash) {
        return token_type::identifier;
    }

    if (std::strncmp(identifier, entry.keyword, size) != 0) {
        return token_type::identifier;
    }

    return entry.type;
}
}
