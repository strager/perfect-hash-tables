
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <nmmintrin.h>

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
  {2073998132U, "from", token_type::kw_from},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3061379300U, "undefined", token_type::kw_undefined},
  {0U, "", token_type::identifier},
  {2364803452U, "default", token_type::kw_default},
  {983078303U, "export", token_type::kw_export},
  {3086927754U, "import", token_type::kw_import},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1102750828U, "assert", token_type::kw_assert},
  {941749439U, "abstract", token_type::kw_abstract},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1332256447U, "return", token_type::kw_return},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {351303325U, "require", token_type::kw_require},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {851092877U, "out", token_type::kw_out},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {849424162U, "class", token_type::kw_class},
  {4218724373U, "try", token_type::kw_try},
  {0U, "", token_type::identifier},
  {1928894500U, "break", token_type::kw_break},
  {3375594671U, "infer", token_type::kw_infer},
  {1951828182U, "null", token_type::kw_null},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3493053541U, "never", token_type::kw_never},
  {0U, "", token_type::identifier},
  {2011704483U, "for", token_type::kw_for},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3071669106U, "true", token_type::kw_true},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {392345861U, "do", token_type::kw_do},
  {328029252U, "extends", token_type::kw_extends},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {244858966U, "intrinsic", token_type::kw_intrinsic},
  {0U, "", token_type::identifier},
  {1723942863U, "as", token_type::kw_as},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3471511928U, "boolean", token_type::kw_boolean},
  {2790799884U, "object", token_type::kw_object},
  {0U, "", token_type::identifier},
  {1387759796U, "override", token_type::kw_override},
  {3754817187U, "of", token_type::kw_of},
  {0U, "", token_type::identifier},
  {270494639U, "protected", token_type::kw_protected},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3455954897U, "void", token_type::kw_void},
  {0U, "", token_type::identifier},
  {2049352459U, "private", token_type::kw_private},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1700084318U, "typeof", token_type::kw_typeof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3404605561U, "while", token_type::kw_while},
  {0U, "", token_type::identifier},
  {2339055933U, "unique", token_type::kw_unique},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1650093366U, "any", token_type::kw_any},
  {1769246197U, "super", token_type::kw_super},
  {0U, "", token_type::identifier},
  {1221387879U, "const", token_type::kw_const},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2881149478U, "new", token_type::kw_new},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4039937320U, "static", token_type::kw_static},
  {0U, "", token_type::identifier},
  {2046372567U, "module", token_type::kw_module},
  {4122331648U, "bigint", token_type::kw_bigint},
  {0U, "", token_type::identifier},
  {3181525470U, "global", token_type::kw_global},
  {0U, "", token_type::identifier},
  {403511357U, "asserts", token_type::kw_asserts},
  {771852993U, "switch", token_type::kw_switch},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3268154586U, "false", token_type::kw_false},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2832472812U, "is", token_type::kw_is},
  {2003588428U, "var", token_type::kw_var},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {436969670U, "delete", token_type::kw_delete},
  {0U, "", token_type::identifier},
  {37409917U, "function", token_type::kw_function},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3947741594U, "yield", token_type::kw_yield},
  {2465072145U, "public", token_type::kw_public},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3769245426U, "package", token_type::kw_package},
  {0U, "", token_type::identifier},
  {157926758U, "namespace", token_type::kw_namespace},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {4030190756U, "await", token_type::kw_await},
  {4088336622U, "finally", token_type::kw_finally},
  {275348908U, "interface", token_type::kw_interface},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3400960157U, "string", token_type::kw_string},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {639253295U, "set", token_type::kw_set},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3025976289U, "get", token_type::kw_get},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {926069908U, "this", token_type::kw_this},
  {970053074U, "declare", token_type::kw_declare},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {505424719U, "enum", token_type::kw_enum},
  {1516413020U, "throw", token_type::kw_throw},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2438052894U, "instanceof", token_type::kw_instanceof},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3245312009U, "async", token_type::kw_async},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {171062613U, "continue", token_type::kw_continue},
  {0U, "", token_type::identifier},
  {1532423975U, "constructor", token_type::kw_constructor},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {1462620542U, "in", token_type::kw_in},
  {0U, "", token_type::identifier},
  {1384455904U, "else", token_type::kw_else},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3560294408U, "catch", token_type::kw_catch},
  {0U, "", token_type::identifier},
  {3392737990U, "if", token_type::kw_if},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {2677838610U, "unknown", token_type::kw_unknown},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {540692798U, "let", token_type::kw_let},
  {1073392644U, "readonly", token_type::kw_readonly},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3278059409U, "number", token_type::kw_number},
  {0U, "", token_type::identifier},
  {1365325576U, "implements", token_type::kw_implements},
  {2357904662U, "type", token_type::kw_type},
  {2957693478U, "with", token_type::kw_with},
  {1881514939U, "keyof", token_type::kw_keyof},
  {2529168530U, "case", token_type::kw_case},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {641255633U, "debugger", token_type::kw_debugger},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {0U, "", token_type::identifier},
  {3322180034U, "symbol", token_type::kw_symbol},

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

    int comparison = std::strncmp(identifier, entry.keyword, size);

    if (comparison == 0) {
        return entry.type;
    } else {
        return token_type::identifier;
    }

}
}
