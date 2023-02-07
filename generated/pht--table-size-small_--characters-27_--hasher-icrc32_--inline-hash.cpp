
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

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
  {762260415U, "do", token_type::kw_do},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {731380503U, "true", token_type::kw_true},
  {768794397U, "extends", token_type::kw_extends},
  {1139095824U, "private", token_type::kw_private},
  {3990137139U, "function", token_type::kw_function},
  {4080987669U, "infer", token_type::kw_infer},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1964116685U, "abstract", token_type::kw_abstract},
  {2739465040U, "return", token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2926902119U, "namespace", token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {2863609364U, "of", token_type::kw_of},
  {0U, "", token_type::identifier},
  {4187408843U, "default", token_type::kw_default},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3005514539U, "constructor", token_type::kw_constructor},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2164723184U, "await", token_type::kw_await},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2231946047U, "override", token_type::kw_override},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3872416776U, "class", token_type::kw_class},
  {0U, "", token_type::identifier},
  {541215366U, "finally", token_type::kw_finally},
  {3697427052U, "instanceof", token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3480860070U, "boolean", token_type::kw_boolean},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3856418843U, "continue", token_type::kw_continue},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2074445480U, "module", token_type::kw_module},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3670591873U, "out", token_type::kw_out},
  {2782172507U, "export", token_type::kw_export},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1251503283U, "readonly", token_type::kw_readonly},
  {0U, "", token_type::identifier},
  {664412990U, "delete", token_type::kw_delete},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3336611897U, "throw", token_type::kw_throw},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {210328094U, "assert", token_type::kw_assert},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4174381938U, "require", token_type::kw_require},
  {0U, "", token_type::identifier},
  {1989373246U, "const", token_type::kw_const},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3647571177U, "package", token_type::kw_package},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2317877562U, "any", token_type::kw_any},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3460998365U, "debugger", token_type::kw_debugger},
  {2589732399U, "unknown", token_type::kw_unknown},
  {3589382205U, "declare", token_type::kw_declare},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {950049778U, "yield", token_type::kw_yield},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1098862361U, "case", token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1195009954U, "is", token_type::kw_is},
  {3158499439U, "while", token_type::kw_while},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4192732577U, "type", token_type::kw_type},
  {1517392174U, "async", token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1419872821U, "implements", token_type::kw_implements},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3766528531U, "from", token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2451195955U, "intrinsic", token_type::kw_intrinsic},
  {0U, "", token_type::identifier},
  {3417299227U, "in", token_type::kw_in},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1137227306U, "new", token_type::kw_new},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1538467351U, "number", token_type::kw_number},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2897317704U, "public", token_type::kw_public},
  {2356503134U, "undefined", token_type::kw_undefined},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1932064265U, "catch", token_type::kw_catch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4225046785U, "interface", token_type::kw_interface},
  {0U, "", token_type::identifier},
  {4097717706U, "void", token_type::kw_void},
  {4016677759U, "keyof", token_type::kw_keyof},
  {0U, "", token_type::identifier},
  {359880529U, "false", token_type::kw_false},
  {2977825860U, "as", token_type::kw_as},
  {0U, "", token_type::identifier},
  {943827049U, "never", token_type::kw_never},
  {0U, "", token_type::identifier},
  {1808962575U, "static", token_type::kw_static},
  {3949708414U, "object", token_type::kw_object},
  {2223968344U, "asserts", token_type::kw_asserts},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4231994944U, "protected", token_type::kw_protected},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {319212569U, "try", token_type::kw_try},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2264159690U, "else", token_type::kw_else},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1444830579U, "global", token_type::kw_global},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {626587091U, "string", token_type::kw_string},
  {0U, "", token_type::identifier},
  {3360177458U, "let", token_type::kw_let},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2046905062U, "this", token_type::kw_this},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3462944547U, "set", token_type::kw_set},
  {4054097011U, "super", token_type::kw_super},
  {1765261925U, "bigint", token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1418708095U, "enum", token_type::kw_enum},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1860838502U, "if", token_type::kw_if},
  {2677686447U, "for", token_type::kw_for},
  {0U, "", token_type::identifier},
  {412113U, "break", token_type::kw_break},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {672087901U, "switch", token_type::kw_switch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1546573805U, "get", token_type::kw_get},
  {1670352032U, "import", token_type::kw_import},
  {0U, "", token_type::identifier},
  {1905669611U, "null", token_type::kw_null},
  {0U, "", token_type::identifier},
  {2405799507U, "symbol", token_type::kw_symbol},
  {2669558080U, "var", token_type::kw_var},
  {3250898445U, "unique", token_type::kw_unique},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {741662159U, "typeof", token_type::kw_typeof},
  {726977281U, "with", token_type::kw_with},
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
