// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "pht.h"
#include "token.h"
#include <algorithm>
#include <bit>
#include <cassert>
#include <cerrno>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace pht {
namespace {
enum class table_size_strategy {
    smallest,
    power_of_2,
    fixed_256,  // for hash_strategy::pearson_8
};

enum class hash_strategy {
    fnv1a32,
    xx3_64,
    intel_crc32,
    lehmer,
    lehmer_128,
    pearson_8,
    aes,
};

enum class string_compare_strategy {
    memcmp,
    check_first_then_memcmp,
    cmpestri,
    sse2,
    ptest,
};

struct table_strategy {
    table_size_strategy size_strategy;
    character_selection_mask character_selection;
    hash_strategy hasher;
    string_compare_strategy string_compare;
    hash_to_index_strategy hash_to_index;
    std::optional<unsigned> entry_size;
    bool inline_hash;
    bool cmov;
};

struct keyword_statistics {
    unsigned long min_keyword_size;
    unsigned long max_keyword_size;
    std::vector<character_selection_mask> unique_character_selections;
};

class table_seed {
public:
    void next() {
        this->data += 1;
        this->data_256_byte_initd = false;
    }

    unsigned long get_32() const {
        return this->data;
    }

    const std::uint8_t* get_256_byte() const {
        if (!this->data_256_byte_initd) {
            this->init_data_256_byte();
        }
        return this->data_256_byte;
    }

private:
    [[gnu::noinline]]
    void init_data_256_byte() const {
        std::iota(std::begin(this->data_256_byte), std::end(this->data_256_byte), 0);
        std::mt19937 rng(this->data);
        std::shuffle(std::begin(this->data_256_byte), std::end(this->data_256_byte), rng);
        this->data_256_byte_initd = true;
    }

    unsigned long data = 0x811c9dc5; // FNV-1a 32-bit basis.
    mutable bool data_256_byte_initd = false;
    mutable std::uint8_t data_256_byte[256];
};

struct perfect_hash_table {
    struct table_entry {
        std::string_view keyword = std::string_view();
        std::uint32_t hash = 0;

        // If 'generation' is out of sync with the table's 'generation', then
        // this entry is vacant thus 'keyword' and 'hash' should be ignored.
        int generation = 0;

        bool is_taken(int generation) const {
            return generation == this->generation && !this->keyword.empty();
        }

        void take(std::string_view keyword, std::uint32_t hash, int generation) {
            this->keyword = keyword;
            this->hash = hash;
            this->generation = generation;
        }
    };

    keyword_statistics stats;
    table_seed seed;
    unsigned long table_size;
    table_strategy strategy;
    unsigned entry_size;
    unsigned entry_padding;
    int generation;

    std::vector<table_entry> entries;
};

template <class Hasher>
[[gnu::noinline]]
bool try_add_all_entries(perfect_hash_table& table) {
    struct hash_and_index {
        std::uint32_t hash;
        std::uint32_t index;
    };
    auto make_hash_and_index = [&table](keyword_token kt) -> hash_and_index {
        std::optional<Hasher> optional_hasher;
        if constexpr (std::is_same_v<Hasher, pearson_8_hasher>) {
            optional_hasher.emplace(table.seed.get_256_byte());
        } else {
            optional_hasher.emplace(table.seed.get_32());
        }
        Hasher& hasher = *optional_hasher;
        hash_selected_characters(table.strategy.character_selection, hasher, kt.keyword.data(), kt.keyword.size());
        std::uint32_t h = hasher.hash();
        std::uint32_t index = hash_to_index(h, table.table_size, table.entry_size, table.strategy.hash_to_index);
        return hash_and_index{
            .hash = h,
            .index = index,
        };
    };

    int generation = table.generation;
    std::size_t i;
    for (i = 0; i + 4 <= std::size(keyword_tokens); i += 4) {
        // Do several at a time for parallelism.
        std::array<hash_and_index, 4> hashes_and_indexes;
#pragma GCC unroll 4
        for (std::size_t j = 0; j < hashes_and_indexes.size(); ++j) {
            hashes_and_indexes[j] = make_hash_and_index(keyword_tokens[i + j]);
        }
        for (std::size_t j = 0; j < hashes_and_indexes.size(); ++j) {
            hash_and_index hi = hashes_and_indexes[j];
            perfect_hash_table::table_entry &entry = table.entries[hi.index];
            if (entry.is_taken(generation)) {
                return false;
            }
            entry.take(keyword_tokens[i + j].keyword, hi.hash, generation);
        }
    }
    for (; i < std::size(keyword_tokens); i += 1) {
        keyword_token kt = keyword_tokens[i];
        hash_and_index hi = make_hash_and_index(kt);
        perfect_hash_table::table_entry &entry = table.entries[hi.index];
        if (entry.is_taken(generation)) {
            return false;
        }
        entry.take(kt.keyword, hi.hash, generation);
    }

    return true;
}

bool try_add_all_entries(perfect_hash_table& table, hash_strategy hasher) {
    switch (hasher) {
        case hash_strategy::fnv1a32:
            return try_add_all_entries<fnv1a32>(table);
        case hash_strategy::xx3_64:
            return try_add_all_entries<xx3_64_hasher>(table);
        case hash_strategy::intel_crc32:
            return try_add_all_entries<intel_crc32_intrinsic_hasher>(table);
        case hash_strategy::lehmer:
            return try_add_all_entries<lehmer_hasher>(table);
        case hash_strategy::lehmer_128:
            return try_add_all_entries<lehmer_128_hasher>(table);
        case hash_strategy::pearson_8:
            return try_add_all_entries<pearson_8_hasher>(table);
        case hash_strategy::aes:
            return try_add_all_entries<aes_intrinsic_hasher>(table);
    }
    __builtin_unreachable();
}

// Returns the number of attempts. If it's >= max_attempts, then building the
// table failed.
[[gnu::noinline]]
int try_build_table(perfect_hash_table& table, int max_attempts) {
    table.seed = table_seed();
    table.entries.clear();  // Reset generations.
    table.entries.resize(table.table_size);
    int attempts = 0;
    for (;;) {
        attempts += 1;
        table.generation = attempts;
        if (attempts >= max_attempts) {
            return attempts;
        }
        bool succeeded = try_add_all_entries(table, table.strategy.hasher);
        if (succeeded) {
            return attempts;
        }
        table.seed.next();
    }
}

[[gnu::noinline]]
keyword_statistics make_stats() {
    keyword_statistics stats;
    stats.min_keyword_size = 0xffffffff;
    stats.max_keyword_size = 0;
    for (keyword_token kt : keyword_tokens) {
        unsigned long size = kt.keyword.size();
        stats.min_keyword_size = std::min(stats.min_keyword_size, size);
        stats.max_keyword_size = std::max(stats.max_keyword_size, size);
    }

    for (character_selection_mask m = 1; m < (1<<5); ++m) {
        std::set<std::string> selections;
        for (keyword_token kt : keyword_tokens) {
            std::string selection;
            std::size_t size = kt.keyword.size();
            if (m & (1 << 0)) selection += kt.keyword[0];
            if (m & (1 << 1)) selection += kt.keyword[1];
            if (m & (1 << 2)) selection += kt.keyword[size-1];
            if (m & (1 << 3)) selection += kt.keyword[size-2];
            if (m & (1 << 4)) selection += size;
            assert(!selection.empty());
            if (selections.count(selection) > 0) {
                // Selection is not unique. Skip.
                goto next_character_selection;
            }
            selections.insert(selection);
        }
        stats.unique_character_selections.push_back(m);
next_character_selection:;
    }
    return stats;
}

perfect_hash_table make_perfect_hash_table(const keyword_statistics& stats, table_strategy strategy) {
    perfect_hash_table table;
    table.stats = stats;

    table.strategy = strategy;

    // TODO(strager): Properly compute this. This is only correct sometimes.
    unsigned stock_entry_size = 13;
    table.entry_size = strategy.entry_size.has_value() ? *strategy.entry_size : stock_entry_size;
    table.entry_padding = table.entry_size - stock_entry_size;

    unsigned long max_table_size = std::size(keyword_tokens) * 65536;
    switch (strategy.size_strategy) {
        case table_size_strategy::smallest:
            table.table_size = std::size(keyword_tokens);
            break;
        case table_size_strategy::power_of_2:
            table.table_size = std::bit_ceil(std::size(keyword_tokens));
            break;
        case table_size_strategy::fixed_256:
            table.table_size = 256;
            break;
    }
    for (;;) {
        if (table.table_size > max_table_size) {
fail:
            std::fprintf(
                stderr,
                "can't generate table of size %lu from %zu items\n",
                table.table_size,
                std::size(keyword_tokens));
            std::exit(1);
        }
        int max_attempts_per_size = 50'000;
        int attempts = try_build_table(table, max_attempts_per_size);
        if (attempts < max_attempts_per_size) {
            std::fprintf(
                stderr,
                "took %d attempts to generate table of size %lu from %zu items\n",
                attempts,
                table.table_size,
                std::size(keyword_tokens));
            break;
        }
        switch (strategy.size_strategy) {
            case table_size_strategy::smallest:
                table.table_size += 1;
                break;
            case table_size_strategy::power_of_2:
                table.table_size *= 2;
                break;
            case table_size_strategy::fixed_256:
                goto fail;
        }
    }

    return table;
}

void write_table(FILE* file, const perfect_hash_table& table) {
    std::fprintf(file, R"(
#include "../pht.h"
#include "../token.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <nmmintrin.h>

namespace pht {
namespace {
constexpr character_selection_mask character_selection = %uU;
constexpr std::uint32_t table_size = %luUL;
constexpr std::size_t min_keyword_size = %lu;
constexpr std::size_t max_keyword_size = %lu;
)", table.strategy.character_selection, table.table_size, table.stats.min_keyword_size, table.stats.max_keyword_size);

    switch (table.strategy.hasher) {
        case hash_strategy::pearson_8:
            std::fprintf(file, "constexpr std::uint8_t hash_seed[] = {\n   ");
            for (int i = 0; i < 256; ++i) {
                std::fprintf(file, " 0x%02x,", table.seed.get_256_byte()[i]);
            }
            std::fprintf(file, "\n};\n");
            break;
        default:
            std::fprintf(file, "constexpr std::uint32_t hash_seed = %luUL;\n", table.seed.get_32());
            break;
    }

    std::fprintf(file, "%s", R"(

struct table_entry {
)");
    if (table.strategy.inline_hash) {
        std::fprintf(file, R"(
    std::uint32_t hash;
)");
    }
    bool need_null_terminator = true;
    std::fprintf(file, R"(
    const char keyword[max_keyword_size + %d];
    token_type type;
)", need_null_terminator ? 1 : 0);
    if (table.entry_padding > 0) {
        std::fprintf(file, "    char padding[%u];\n", table.entry_padding);
    }
    std::fprintf(file, "};\n");
    // TODO(strager): Always add this static_assert.
    if (table.strategy.entry_size.has_value()) {
        std::fprintf(file, "static_assert(sizeof(table_entry) == %u);\n", table.entry_size);
    }

    std::fprintf(file, R"(
constexpr table_entry table[table_size] = {
)");

    int keyword_buffer_size = table.stats.max_keyword_size + (need_null_terminator ? 1 : 0);
    std::vector<char> keyword_buffer;
    keyword_buffer.resize(keyword_buffer_size);
    for (const perfect_hash_table::table_entry& entry : table.entries) {
        std::fprintf(file, "  {");
        if (!entry.is_taken(table.generation)) {
            if (table.strategy.inline_hash) {
                std::fprintf(file, "0U, ");
            }
            std::fprintf(file, "\"\", token_type::identifier},\n");
        } else {
            if (table.strategy.inline_hash) {
                std::fprintf(file, "%luU, ", (unsigned long)entry.hash);
            }

            // Pad with non-zero bytes so our length checks work properly.
            std::memset(keyword_buffer.data(), '_', keyword_buffer_size);
            std::memcpy(keyword_buffer.data(), entry.keyword.data(), entry.keyword.size());
            if (need_null_terminator) {
                keyword_buffer[entry.keyword.size()] = '\0';
            }

            // Write the string character-by-character to avoid C++ adding a
            // null terminator for us.
            std::fprintf(file, "{");
            for (char c : keyword_buffer) {
                if (c == '\0') {
                    std::fprintf(file, "0,");
                } else {
                    std::fprintf(file, "'%c',", c);
                }
            }
            std::fprintf(
                    file, "}, token_type::kw_%*s},\n",
                    (int)entry.keyword.size(), entry.keyword.data());
        }
    }

    const char* hasher_class;
    switch (table.strategy.hasher) {
        case hash_strategy::xx3_64: hasher_class = "xx3_64_hasher"; break;
        case hash_strategy::fnv1a32: hasher_class = "fnv1a32"; break;
        case hash_strategy::intel_crc32: hasher_class = "intel_crc32_intrinsic_hasher"; break;
        case hash_strategy::lehmer: hasher_class = "lehmer_hasher"; break;
        case hash_strategy::lehmer_128: hasher_class = "lehmer_128_hasher"; break;
        case hash_strategy::pearson_8: hasher_class = "pearson_8_hasher"; break;
        case hash_strategy::aes: hasher_class = "aes_intrinsic_hasher"; break;
    }
    const char* hash_to_index;
    switch (table.strategy.hash_to_index) {
        case hash_to_index_strategy::modulo: hash_to_index = "modulo"; break;
        case hash_to_index_strategy::shiftless: hash_to_index = "shiftless"; break;
    }
    std::fprintf(file, R"(
};

constexpr std::uint8_t t = 0xff;
constexpr std::uint8_t f = 0x00;
constexpr std::uint8_t masks[max_keyword_size+1][16] = {
    {f,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f},
    {t,f,f,f,f,f,f,f,f,f,f,f,f,f,f,f},
    {t,t,f,f,f,f,f,f,f,f,f,f,f,f,f,f},
    {t,t,t,f,f,f,f,f,f,f,f,f,f,f,f,f},
    {t,t,t,t,f,f,f,f,f,f,f,f,f,f,f,f},
    {t,t,t,t,t,f,f,f,f,f,f,f,f,f,f,f},
    {t,t,t,t,t,t,f,f,f,f,f,f,f,f,f,f},
    {t,t,t,t,t,t,t,f,f,f,f,f,f,f,f,f},
    {t,t,t,t,t,t,t,t,f,f,f,f,f,f,f,f},
    {t,t,t,t,t,t,t,t,t,f,f,f,f,f,f,f},
    {t,t,t,t,t,t,t,t,t,t,f,f,f,f,f,f},
    {t,t,t,t,t,t,t,t,t,t,t,f,f,f,f,f},
};
}

token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {
    if (size < min_keyword_size || size > max_keyword_size) {
        return token_type::identifier;
    }

    %s hasher(hash_seed);
    hash_selected_characters(character_selection, hasher, identifier, size);
    std::uint32_t h = hasher.hash();
    std::uint32_t index = hash_to_index(h, table_size, sizeof(table_entry), hash_to_index_strategy::%s);
)", hasher_class, hash_to_index);
    std::fprintf(file, "    const table_entry& entry = table[index];\n");
    if (table.strategy.inline_hash) {
        std::fprintf(file, "%s", R"(
    if (h != entry.hash) {
        return token_type::identifier;
    }
)");
    }
    switch (table.strategy.string_compare) {
        case string_compare_strategy::check_first_then_memcmp:
            std::fprintf(file, "%s", R"(
    if (entry.keyword[0] != identifier[0]) {
        return token_type::identifier;
    }
    int comparison = std::memcmp(identifier + 1, entry.keyword + 1, size - 1);
    if (comparison == 0) {
        comparison = entry.keyword[size];  // length check
    }
)");
            break;
        case string_compare_strategy::memcmp:
            std::fprintf(file, "%s", R"(
    int comparison = std::memcmp(identifier, entry.keyword, size);
    if (comparison == 0) {
        comparison = entry.keyword[size];  // length check
    }
)");
            break;
        case string_compare_strategy::sse2:
                std::fprintf(file, "%s", R"(
    __m128i mask = ::_mm_load_si128((const __m128i*)masks[size]);
    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry.keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)identifier);
    __m128i entry_masked = ::_mm_and_si128(entry_unmasked, mask);
    __m128i identifier_masked = ::_mm_and_si128(identifier_unmasked, mask);
    // FIXME(strager): LLVM likes to optimize this to a 'ptest', breaking
    // fairness. We should compile without -msse4.2.
    int comparison = ::_mm_movemask_epi8(::_mm_cmpeq_epi8(entry_masked, identifier_masked)) ^ 0xffff;
    if (comparison == 0) {
        comparison = entry.keyword[size];  // length check
    }
)");
            break;
        case string_compare_strategy::ptest:
            std::fprintf(file, "%s", R"(
    __m128i mask = ::_mm_load_si128((const __m128i*)masks[size]);
    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry.keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)identifier);
    int comparison = ::_mm_test_all_zeros(mask, ::_mm_xor_si128(entry_unmasked, identifier_unmasked)) != 1;
    if (comparison == 0) {
        comparison = entry.keyword[size];  // length check
    }
)");
            break;
        case string_compare_strategy::cmpestri:
            if (table.strategy.cmov) {
                std::fprintf(file, "%s", R"(
    int result = (int)entry.type;
    __asm__(
        // If what should be the null terminator is not null, then
        // (size != strlen(entry.keyword)), so set result to
        // token_type::identifier.
        "cmpb $0, %[entry_keyword_at_size]\n"
        "cmovne %[token_type_identifier], %[result]\n"

        : [result]"+r"(result)

        : [entry_keyword_at_size]"m"(entry.keyword[size]),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by pcmpestrm and cmp.
    );

    __asm__(
        // Compare the entry.keyword and identifier strings.
        // %eax: size of %[entry_keyword].
        // %edx: size of %[identifier].
        "pcmpestrm %[cmpestrm_flags], %[entry_keyword], %[identifier]\n"
        // Move if cmpestr's mask was non-zero.
        "cmovc %[token_type_identifier], %[result]\n"

        : [result]"+r"(result)

        : [identifier]"x"(::_mm_lddqu_si128((const __m128i*)identifier)),
          [entry_keyword]"x"(::_mm_lddqu_si128((const __m128i*)entry.keyword)),
          [token_type_identifier]"r"((int)token_type::identifier),
          [cmpestrm_flags]"i"(_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY),
          "a"(size), // %eax: size of %[entry_keyword].
          "d"(size)  // %edx: size of %[identifier].

        : "cc",   // Clobbered by pcmpestrm.
          "xmm0"  // Clobbered by pcmpestrm.
    );
    return (token_type)result;
)");
            } else {
                std::fprintf(file, "%s", R"(
    int comparison = _mm_cmpestrc(
        ::_mm_lddqu_si128((const __m128i*)identifier),
        size,
        ::_mm_lddqu_si128((const __m128i*)entry.keyword),
        size,
        _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY);
    if (comparison == 0) {
        comparison = entry.keyword[size];  // length check
    }
)");
            }
            break;
    }
    if (!table.strategy.cmov) {
        std::fprintf(file, "%s", R"(
    if (comparison == 0) {
        return entry.type;
    } else {
        return token_type::identifier;
    }
)");
    }
    std::fprintf(file, "%s", R"(
}
}
)");
}

void write_table(const char* file_path, const perfect_hash_table& table) {
    FILE* file = std::fopen(file_path, "wb");
    if (file == nullptr) {
        std::fprintf(stderr, "error: failed to open %s: %s\n", file_path, std::strerror(errno));
        std::exit(1);
    }

    write_table(file, table);

    std::fclose(file);
}

void go(int argc, char** argv) {
    auto look_up_or_die = [](const char* input, const char* what, const auto& map) -> auto {
        auto it = map.find(input);
        if (it == map.end()) {
            std::fprintf(stderr, "error: cannot parse %s: %s", what, input);
            std::exit(1);
        }
        return it->second;
    };

    static constexpr ::option long_options[] = {
        {"characters",      required_argument, 0, 'c' },
        {"entry-size",      required_argument, 0, 'z' },
        {"hasher",          required_argument, 0, 'h' },
        {"output",          required_argument, 0, 'o' },
        {"string-compare",  required_argument, 0, 's' },
        {"table-size",      required_argument, 0, 't' },
        {"cmov",            no_argument,       0, 'C' },
        {"inline-hash",     no_argument,       0, 'i' },
        {"shiftless-index", no_argument,       0, 'l' },
        {nullptr,           0,                 0, 0   }
    };

    const char* out_file_path = nullptr;
    bool inline_hash = false;
    bool cmov = false;
    hash_to_index_strategy hash_to_index = hash_to_index_strategy::modulo;
    string_compare_strategy string_compare = string_compare_strategy::memcmp;
    std::optional<table_size_strategy> size_strategy;
    std::optional<character_selection_mask> character_selection;
    std::optional<hash_strategy> hasher;
    std::optional<unsigned> entry_size;

    for (;;) {
        int long_index = 0;
        int opt = getopt_long_only(argc, argv, "", long_options, &long_index);
        switch (opt) {
            case 'C':
                cmov = true;
                break;

            case 'c': {
                character_selection.emplace();
                const char* optarg_end = optarg + std::strlen(optarg);
                std::from_chars_result r = std::from_chars(optarg, optarg_end, *character_selection);
                if (r.ptr != optarg_end || r.ec != std::errc()) {
                    std::fprintf(stderr, "error: cannot parse character selection: %s", optarg);
                    std::exit(1);
                }
                break;
            }

            case 'z': {
                entry_size.emplace();
                const char* optarg_end = optarg + std::strlen(optarg);
                std::from_chars_result r = std::from_chars(optarg, optarg_end, *entry_size);
                if (r.ptr != optarg_end || r.ec != std::errc()) {
                    std::fprintf(stderr, "error: cannot parse entry size: %s", optarg);
                    std::exit(1);
                }
                break;
            }

            case 'h':
                hasher = look_up_or_die(optarg, "--hasher", std::map<std::string_view, hash_strategy>{
                    {"fnv1a32", hash_strategy::fnv1a32},
                    {"xx364", hash_strategy::xx3_64},
                    {"icrc32", hash_strategy::intel_crc32},
                    {"lehmer", hash_strategy::lehmer},
                    {"lehmer128", hash_strategy::lehmer_128},
                    {"pearson8", hash_strategy::pearson_8},
                    {"aes", hash_strategy::aes},
                });
                break;

            case 'i':
                inline_hash = true;
                break;

            case 'l':
                hash_to_index = hash_to_index_strategy::shiftless;
                break;

            case 's':
                string_compare = look_up_or_die(optarg, "--string-compare", std::map<std::string_view, string_compare_strategy>{
                    {"memcmp", string_compare_strategy::memcmp},
                    {"check1memcmp", string_compare_strategy::check_first_then_memcmp},
                    {"cmpestri", string_compare_strategy::cmpestri},
                    {"sse2", string_compare_strategy::sse2},
                    {"ptest", string_compare_strategy::ptest},
                });
                break;

            case 't':
                size_strategy = look_up_or_die(optarg, "--table-size", std::map<std::string_view, table_size_strategy>{
                    {"small", table_size_strategy::smallest},
                    {"pot", table_size_strategy::power_of_2},
                });
                break;

            case 'o':
                out_file_path = optarg;
                break;

            case -1:
                goto done_parsing;

            default:
                std::fprintf(stderr, "error: invalid option\n");
                std::exit(1);
                break;
        }
    }
done_parsing:

    if (!character_selection.has_value()) {
        std::fprintf(stderr, "error: missing required --characters\n");
        std::exit(1);
    }
    if (!hasher.has_value()) {
        std::fprintf(stderr, "error: missing required --hasher\n");
        std::exit(1);
    }
    if (!size_strategy.has_value() && hasher != hash_strategy::pearson_8) {
        std::fprintf(stderr, "error: missing required --table-size\n");
        std::exit(1);
    }

    if (*hasher == hash_strategy::pearson_8) {
        if (size_strategy.has_value()) {
            std::fprintf(stderr, "error: --table-size incompatible with --hasher=pearson8\n");
            std::exit(1);
        }
        size_strategy = table_size_strategy::fixed_256;
        if (inline_hash) {
            std::fprintf(stderr, "error: --inline-hash incompatible with --hasher=pearson8\n");
            std::exit(1);
        }
    }

    if (hash_to_index == hash_to_index_strategy::shiftless && size_strategy != table_size_strategy::power_of_2) {
        std::fprintf(stderr, "error: --shiftless-index requires --table-size=pot\n");
        std::exit(1);
    }

    table_strategy strategy = {
        .size_strategy = *size_strategy,
        .character_selection = *character_selection,
        .hasher = *hasher,
        .string_compare = string_compare,
        .hash_to_index = hash_to_index,
        .entry_size = entry_size,
        .inline_hash = inline_hash,
        .cmov = cmov,
    };

    keyword_statistics stats = make_stats();
    if (std::count(stats.unique_character_selections.begin(), stats.unique_character_selections.end(), strategy.character_selection) == 0) {
        std::fprintf(stderr, "error: character selection is not unique: %u", strategy.character_selection);
        std::exit(1);
    }

    write_table(out_file_path, make_perfect_hash_table(stats, strategy));
}
}
}

int main(int argc, char** argv) {
    pht::go(argc, argv);
    return 0;
}

// quick-lint-js finds bugs in JavaScript programs.
// Copyright (C) 2020  Matthew "strager" Glazar
//
// This file is part of quick-lint-js.
//
// quick-lint-js is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// quick-lint-js is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with quick-lint-js.  If not, see <https://www.gnu.org/licenses/>.
