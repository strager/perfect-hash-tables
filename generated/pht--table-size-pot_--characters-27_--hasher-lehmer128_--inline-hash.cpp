
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace pht {
namespace {
constexpr character_selection_mask character_selection = 27U;
constexpr std::uint32_t hash_seed = 2166138572UL;
constexpr std::uint32_t table_size = 512UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;


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
  {1911449605U, "else", token_type::kw_else},
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
  {1883303455U, "do", token_type::kw_do},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1983297058U, "constructor", token_type::kw_constructor},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1954759206U, "intrinsic", token_type::kw_intrinsic},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1925550126U, "unknown", token_type::kw_unknown},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1968412728U, "continue", token_type::kw_continue},
  {0U, "", token_type::identifier},
  {1897347130U, "keyof", token_type::kw_keyof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1939705918U, "delete", token_type::kw_delete},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1911507523U, "null", token_type::kw_null},
  {1911286340U, "type", token_type::kw_type},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940268107U, "bigint", token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {1911059021U, "with", token_type::kw_with},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1969306709U, "debugger", token_type::kw_debugger},
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
  {1940882529U, "global", token_type::kw_global},
  {0U, "", token_type::identifier},
  {1940993123U, "object", token_type::kw_object},
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
  {1926282350U, "extends", token_type::kw_extends},
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
  {1968974471U, "function", token_type::kw_function},
  {1954983048U, "undefined", token_type::kw_undefined},
  {1940043401U, "import", token_type::kw_import},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897910413U, "break", token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940327569U, "typeof", token_type::kw_typeof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940327575U, "symbol", token_type::kw_symbol},
  {1926445208U, "package", token_type::kw_package},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1898019485U, "catch", token_type::kw_catch},
  {0U, "", token_type::identifier},
  {1883138207U, "is", token_type::kw_is},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940436646U, "static", token_type::kw_static},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1969642668U, "abstract", token_type::kw_abstract},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1883133616U, "if", token_type::kw_if},
  {0U, "", token_type::identifier},
  {1911785138U, "void", token_type::kw_void},
  {1925384883U, "private", token_type::kw_private},
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
  {1925828799U, "default", token_type::kw_default},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940047557U, "export", token_type::kw_export},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897181896U, "yield", token_type::kw_yield},
  {1954985673U, "protected", token_type::kw_protected},
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
  {1969253590U, "override", token_type::kw_override},
  {0U, "", token_type::identifier},
  {1911113432U, "enum", token_type::kw_enum},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1911675617U, "from", token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940268778U, "string", token_type::kw_string},
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
  {1911786229U, "this", token_type::kw_this},
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
  {1925831424U, "finally", token_type::kw_finally},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897295113U, "async", token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897124624U, "const", token_type::kw_const},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1955318547U, "namespace", token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940045594U, "assert", token_type::kw_assert},
  {1868698907U, "for", token_type::kw_for},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1911116067U, "true", token_type::kw_true},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1939762982U, "unique", token_type::kw_unique},
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
  {1968803122U, "readonly", token_type::kw_readonly},
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
  {1869258045U, "var", token_type::kw_var},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1883585862U, "as", token_type::kw_as},
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
  {1897124182U, "false", token_type::kw_false},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897462107U, "await", token_type::kw_await},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1997621599U, "instanceof", token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1926557545U, "boolean", token_type::kw_boolean},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1869033324U, "let", token_type::kw_let},
  {0U, "", token_type::identifier},
  {1869033326U, "new", token_type::kw_new},
  {0U, "", token_type::identifier},
  {1897686384U, "super", token_type::kw_super},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1869033334U, "get", token_type::kw_get},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1911448955U, "case", token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940436862U, "public", token_type::kw_public},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1925385089U, "asserts", token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1883135365U, "in", token_type::kw_in},
  {1883021702U, "of", token_type::kw_of},
  {1869033351U, "set", token_type::kw_set},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1997006737U, "implements", token_type::kw_implements},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940995989U, "switch", token_type::kw_switch},
  {0U, "", token_type::identifier},
  {1925716887U, "declare", token_type::kw_declare},
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
  {1868421543U, "try", token_type::kw_try},
  {0U, "", token_type::identifier},
  {1925716905U, "require", token_type::kw_require},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897349549U, "throw", token_type::kw_throw},
  {1897682862U, "never", token_type::kw_never},
  {1868642735U, "any", token_type::kw_any},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940660664U, "number", token_type::kw_number},
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
  {1940041674U, "return", token_type::kw_return},
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
  {1868141526U, "out", token_type::kw_out},
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
  {1897124842U, "class", token_type::kw_class},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1955318768U, "interface", token_type::kw_interface},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897181683U, "while", token_type::kw_while},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1897683958U, "infer", token_type::kw_infer},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1940154873U, "module", token_type::kw_module},
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
