
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace pht {
namespace {
constexpr character_selection_mask character_selection = 23U;
constexpr std::uint32_t hash_basis = 2166172010UL;
constexpr std::uint32_t table_size = 253UL;
constexpr std::size_t min_keyword_size = 2;
constexpr std::size_t max_keyword_size = 11;


struct table_entry {

    const char keyword[max_keyword_size + 1];
    token_type type;
};

constexpr table_entry table[table_size] = {
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"do", token_type::kw_do},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"true", token_type::kw_true},
  {"extends", token_type::kw_extends},
  {"private", token_type::kw_private},
  {"function", token_type::kw_function},
  {"infer", token_type::kw_infer},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"abstract", token_type::kw_abstract},
  {"return", token_type::kw_return},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"namespace", token_type::kw_namespace},
  {"", token_type::identifier},
  {"of", token_type::kw_of},
  {"", token_type::identifier},
  {"default", token_type::kw_default},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"constructor", token_type::kw_constructor},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"await", token_type::kw_await},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"override", token_type::kw_override},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"class", token_type::kw_class},
  {"", token_type::identifier},
  {"finally", token_type::kw_finally},
  {"instanceof", token_type::kw_instanceof},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"boolean", token_type::kw_boolean},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"continue", token_type::kw_continue},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"module", token_type::kw_module},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"out", token_type::kw_out},
  {"export", token_type::kw_export},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"readonly", token_type::kw_readonly},
  {"", token_type::identifier},
  {"delete", token_type::kw_delete},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"throw", token_type::kw_throw},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"assert", token_type::kw_assert},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"require", token_type::kw_require},
  {"", token_type::identifier},
  {"const", token_type::kw_const},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"package", token_type::kw_package},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"any", token_type::kw_any},
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
  {"debugger", token_type::kw_debugger},
  {"unknown", token_type::kw_unknown},
  {"declare", token_type::kw_declare},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"yield", token_type::kw_yield},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"case", token_type::kw_case},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"is", token_type::kw_is},
  {"while", token_type::kw_while},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"type", token_type::kw_type},
  {"async", token_type::kw_async},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"implements", token_type::kw_implements},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"from", token_type::kw_from},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"intrinsic", token_type::kw_intrinsic},
  {"", token_type::identifier},
  {"in", token_type::kw_in},
  {"", token_type::identifier},
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
  {"number", token_type::kw_number},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"public", token_type::kw_public},
  {"undefined", token_type::kw_undefined},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"catch", token_type::kw_catch},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"interface", token_type::kw_interface},
  {"", token_type::identifier},
  {"void", token_type::kw_void},
  {"keyof", token_type::kw_keyof},
  {"", token_type::identifier},
  {"false", token_type::kw_false},
  {"as", token_type::kw_as},
  {"", token_type::identifier},
  {"never", token_type::kw_never},
  {"", token_type::identifier},
  {"static", token_type::kw_static},
  {"object", token_type::kw_object},
  {"asserts", token_type::kw_asserts},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"protected", token_type::kw_protected},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"try", token_type::kw_try},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"else", token_type::kw_else},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"global", token_type::kw_global},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"string", token_type::kw_string},
  {"", token_type::identifier},
  {"let", token_type::kw_let},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"this", token_type::kw_this},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"set", token_type::kw_set},
  {"super", token_type::kw_super},
  {"bigint", token_type::kw_bigint},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"enum", token_type::kw_enum},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"if", token_type::kw_if},
  {"for", token_type::kw_for},
  {"", token_type::identifier},
  {"break", token_type::kw_break},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"switch", token_type::kw_switch},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"get", token_type::kw_get},
  {"import", token_type::kw_import},
  {"", token_type::identifier},
  {"null", token_type::kw_null},
  {"", token_type::identifier},
  {"symbol", token_type::kw_symbol},
  {"var", token_type::kw_var},
  {"unique", token_type::kw_unique},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"", token_type::identifier},
  {"typeof", token_type::kw_typeof},
  {"with", token_type::kw_with},
  {"", token_type::identifier},
  {"", token_type::identifier},

};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {
    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    intel_crc32_intrinsic_hasher hasher(hash_basis);
    hash_selected_characters(character_selection, hasher, identifier, size);
    std::uint32_t h = hasher.hash();
    std::uint32_t index = h % table_size;

    const table_entry& entry = table[index];

    if (std::strncmp(identifier, entry.keyword, size) == 0) {
        return entry.type;
    }

    return token_type::identifier;
}
}