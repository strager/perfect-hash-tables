
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

    const char keyword[max_keyword_size + 1];
    token_type type;
};

constexpr table_entry table[table_size] = {
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"from", token_type::kw_from},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"undefined", token_type::kw_undefined},
  {"", token_type::identifier},
  {"default", token_type::kw_default},
  {"export", token_type::kw_export},
  {"import", token_type::kw_import},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"assert", token_type::kw_assert},
  {"abstract", token_type::kw_abstract},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"return", token_type::kw_return},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"require", token_type::kw_require},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"out", token_type::kw_out},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"class", token_type::kw_class},
  {"try", token_type::kw_try},
  {"", token_type::identifier},
  {"break", token_type::kw_break},
  {"infer", token_type::kw_infer},
  {"null", token_type::kw_null},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"never", token_type::kw_never},
  {"", token_type::identifier},
  {"for", token_type::kw_for},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"true", token_type::kw_true},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"do", token_type::kw_do},
  {"extends", token_type::kw_extends},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"intrinsic", token_type::kw_intrinsic},
  {"", token_type::identifier},
  {"as", token_type::kw_as},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"boolean", token_type::kw_boolean},
  {"object", token_type::kw_object},
  {"", token_type::identifier},
  {"override", token_type::kw_override},
  {"of", token_type::kw_of},
  {"", token_type::identifier},
  {"protected", token_type::kw_protected},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"void", token_type::kw_void},
  {"", token_type::identifier},
  {"private", token_type::kw_private},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"typeof", token_type::kw_typeof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"while", token_type::kw_while},
  {"", token_type::identifier},
  {"unique", token_type::kw_unique},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"any", token_type::kw_any},
  {"super", token_type::kw_super},
  {"", token_type::identifier},
  {"const", token_type::kw_const},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"new", token_type::kw_new},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"static", token_type::kw_static},
  {"", token_type::identifier},
  {"module", token_type::kw_module},
  {"bigint", token_type::kw_bigint},
  {"", token_type::identifier},
  {"global", token_type::kw_global},
  {"", token_type::identifier},
  {"asserts", token_type::kw_asserts},
  {"switch", token_type::kw_switch},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"false", token_type::kw_false},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"is", token_type::kw_is},
  {"var", token_type::kw_var},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"delete", token_type::kw_delete},
  {"", token_type::identifier},
  {"function", token_type::kw_function},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"yield", token_type::kw_yield},
  {"public", token_type::kw_public},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"package", token_type::kw_package},
  {"", token_type::identifier},
  {"namespace", token_type::kw_namespace},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"await", token_type::kw_await},
  {"finally", token_type::kw_finally},
  {"interface", token_type::kw_interface},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"string", token_type::kw_string},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"set", token_type::kw_set},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"get", token_type::kw_get},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"this", token_type::kw_this},
  {"declare", token_type::kw_declare},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"enum", token_type::kw_enum},
  {"throw", token_type::kw_throw},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"instanceof", token_type::kw_instanceof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"async", token_type::kw_async},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"continue", token_type::kw_continue},
  {"", token_type::identifier},
  {"constructor", token_type::kw_constructor},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"in", token_type::kw_in},
  {"", token_type::identifier},
  {"else", token_type::kw_else},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"catch", token_type::kw_catch},
  {"", token_type::identifier},
  {"if", token_type::kw_if},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"unknown", token_type::kw_unknown},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"let", token_type::kw_let},
  {"readonly", token_type::kw_readonly},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"number", token_type::kw_number},
  {"", token_type::identifier},
  {"implements", token_type::kw_implements},
  {"type", token_type::kw_type},
  {"with", token_type::kw_with},
  {"keyof", token_type::kw_keyof},
  {"case", token_type::kw_case},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"debugger", token_type::kw_debugger},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"symbol", token_type::kw_symbol},

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


    if (entry.keyword[0] != identifier[0]) {
        return token_type::identifier;
    }
    bool match = std::strncmp(identifier + 1, entry.keyword + 1, size) == 0;

    if (match) {
        return entry.type;
    } else {
        return token_type::identifier;
    }
}
}
