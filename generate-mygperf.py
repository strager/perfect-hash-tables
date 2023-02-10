# Copyright (C) 2020  Matthew "strager" Glazar
# See end of file for extended copyright information.

import typing
import argparse
import pathlib
import subprocess

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", required=True, metavar="PATH")
    parser.add_argument("--string-compare", default="memcmp")
    parser.add_argument("--cmov", action="store_true")
    args = parser.parse_args()

    output_path = pathlib.Path(args.output)
    cmov = args.cmov

    string_compare = args.string_compare
    legal_string_compares = ["memcmp", "check1memcmp", "sse2", "ptest", "cmpestri"]
    if string_compare not in legal_string_compares:
        raise Exception(f"expected --string-compare={legal_string_compares.join(' or =')}")

    raw_gperf_output = subprocess.check_output(["gperf", "token.gperf"], encoding="utf-8")
    tables = parse_gperf_output(raw_gperf_output)

    with open(output_path, "w") as out:
        out.write(f"""\
#include <cstddef>
#include <cstring>
#include "token.h"
#include <nmmintrin.h>

namespace pht {{
namespace {{
""")
        out.write(tables.constants.replace("{", "constexpr int ").replace("}", "").replace(",", ";\nconstexpr int "))
        out.write(f"""\
struct keyword_entry {{
  char keyword[MAX_WORD_LENGTH + 1];
  token_type type;
}};
}}

token_type look_up_identifier(const char* str, std::size_t len) noexcept {{
    static constexpr std::uint8_t asso_values[] = {tables.asso_values};
    static constexpr keyword_entry wordlist[] = {tables.wordlist};

    if (len > MAX_WORD_LENGTH || len < MIN_WORD_LENGTH) {{
        return token_type::identifier;
    }}

    unsigned h = {tables.hash_function};
    if (h > MAX_HASH_VALUE) {{
        return token_type::identifier;
    }}

    const keyword_entry& entry = wordlist[h];
""")

        if string_compare == "memcmp":
            out.write(f"""\
    int comparison = std::memcmp(str, entry.keyword, len);
    if (comparison == 0) {{
        comparison = entry.keyword[len];  // length check
    }}
""")
        elif string_compare == "check1memcmp":
            out.write(f"""\
    if (entry.keyword[0] != str[0]) {{
        return token_type::identifier;
    }}
    int comparison = std::memcmp(str, entry.keyword, len);
    if (comparison == 0) {{
        comparison = entry.keyword[len];  // length check
    }}
""")
        elif string_compare == "sse2":
            out.write(f"""\
    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry.keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)str);
    // Calculating the mask this way seems to be much much faster than '(1 << len) - 1'.
    std::uint32_t inv_mask = ~(std::uint32_t)0 << len;
    std::uint32_t mask = ~inv_mask;
    std::uint32_t equal_mask = ::_mm_movemask_epi8(::_mm_cmpeq_epi8(entry_unmasked, identifier_unmasked));
    std::uint32_t not_equal_mask = ~equal_mask;
""")
            if cmov:
                out.write(f"""\
    int result = (int)entry.type;
    __asm__(
        // If what should be the null terminator is not null, then
        // (len != strlen(entry.keyword)), so set result to
        // token_type::identifier.
        "cmpb $0, %[entry_keyword_at_size]\\n"
        "cmovne %[token_type_identifier], %[result]\\n"

        : [result]"+r"(result)

        : [entry_keyword_at_size]"m"(entry.keyword[len]),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by cmp.
    );

    __asm__(
        "test %[not_equal_mask], %[mask]\\n"
        "cmovne %[token_type_identifier], %[result]\\n"

        : [result]"+r"(result)

        : [not_equal_mask]"r"(not_equal_mask),
          [mask]"r"(mask),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by test.
    );
    return (token_type)result;
""")
            else:
                out.write(f"""\
    int comparison = mask & ~equal_mask;
    if (comparison == 0) {{
        comparison = entry.keyword[len];  // length check
    }}
""")
        elif string_compare == "ptest":
            out.write(f"""\
    __m128i mask = ::_mm_cmpgt_epi8(
        ::_mm_set1_epi8(len),
        ::_mm_setr_epi8(
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 9, 10, 11, 12, 13, 14, 15));
    __m128i entry_unmasked = ::_mm_lddqu_si128((const __m128i*)entry.keyword);
    __m128i identifier_unmasked = ::_mm_lddqu_si128((const __m128i*)str);
    __m128i compared = ::_mm_xor_si128(entry_unmasked, identifier_unmasked);
""")
            if cmov:
                out.write(f"""\
    int result = (int)entry.type;
    __asm__(
        // If what should be the null terminator is not null, then
        // (size != strlen(entry.keyword)), so set result to
        // token_type::identifier.
        "cmpb $0, %[entry_keyword_at_size]\\n"
        "cmovne %[token_type_identifier], %[result]\\n"

        : [result]"+r"(result)

        : [entry_keyword_at_size]"m"(entry.keyword[len]),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by cmp.
    );

    __asm__(
        // Compare the entry.keyword and identifier strings.
        "ptest %[compared], %[mask]\\n"
        "cmovne %[token_type_identifier], %[result]\\n"

        : [result]"+r"(result)

        : [compared]"x"(compared),
          [mask]"x"(mask),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by ptest.
    );
    return (token_type)result;
""")
            else:
                out.write(f"""\
    int comparison = ::_mm_test_all_zeros(mask, compared) != 1;
    if (comparison == 0) {{
        comparison = entry.keyword[len];  // length check
    }}
""")
        elif string_compare == "cmpestri":
            if cmov:
                out.write(f"""\
    int result = (int)entry.type;
    __asm__(
        // If what should be the null terminator is not null, then
        // (size != strlen(entry.keyword)), so set result to
        // token_type::identifier.
        "cmpb $0, %[entry_keyword_at_size]\\n"
        "cmovne %[token_type_identifier], %[result]\\n"

        : [result]"+r"(result)

        : [entry_keyword_at_size]"m"(entry.keyword[len]),
          [token_type_identifier]"r"((int)token_type::identifier)

        : "cc"   // Clobbered by pcmpestrm and cmp.
    );

    __asm__(
        // Compare the entry.keyword and identifier strings.
        // %eax: size of %[entry_keyword].
        // %edx: size of %[identifier].
        "pcmpestrm %[cmpestrm_flags], %[entry_keyword], %[identifier]\\n"
        // Move if cmpestr's mask was non-zero.
        "cmovc %[token_type_identifier], %[result]\\n"

        : [result]"+r"(result)

        : [identifier]"x"(::_mm_lddqu_si128((const __m128i*)str)),
          [entry_keyword]"x"(::_mm_lddqu_si128((const __m128i*)entry.keyword)),
          [token_type_identifier]"r"((int)token_type::identifier),
          [cmpestrm_flags]"i"(_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY),
          "a"(len), // %eax: size of %[entry_keyword].
          "d"(len)  // %edx: size of %[identifier].

        : "cc",   // Clobbered by pcmpestrm.
          "xmm0"  // Clobbered by pcmpestrm.
    );
    return (token_type)result;
""")
            else:
                out.write(f"""\
    int comparison = _mm_cmpestrc(
        ::_mm_lddqu_si128((const __m128i*)str),
        len,
        ::_mm_lddqu_si128((const __m128i*)entry.keyword),
        len,
        _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY);
    if (comparison == 0) {{
        comparison = entry.keyword[len];  // length check
    }}
""")

        if not cmov:
            out.write(f"""\
    if (comparison == 0) {{
        return wordlist[h].type;
    }} else {{
        return token_type::identifier;
    }}
""")
        out.write(f"""\
}}
}}
""")

class GPerfTables(typing.NamedTuple):
    asso_values: str
    hash_function: str
    constants: str
    wordlist: str

def parse_gperf_output(c: str) -> GPerfTables:
    asso_values: str = ""
    hash_function: str = ""
    constants: str = ""
    wordlist: str = ""

    state = None

    for line in c.splitlines():
        if "#line" in line:
            continue
        if state is None:
            if "asso_values[]" in line:
                state = "asso_values"
            elif "return len" in line:
                hash_function = line.replace("return ", "").rstrip(";")
            elif line == "  enum":
                state = "constants"
            elif "wordlist[]" in line:
                state = "wordlist"
        elif state == "asso_values":
            asso_values += line
            if "};" in line:
                state = None
        elif state == "constants":
            constants += line
            if "};" in line:
                state = None
        elif state == "wordlist":
            wordlist += line
            if "};" in line:
                state = None

    return GPerfTables(
        asso_values=asso_values,
        hash_function=hash_function,
        constants=constants,
        wordlist=wordlist,
    )

if __name__ == "__main__":
    main()

# quick-lint-js finds bugs in JavaScript programs.
# Copyright (C) 2020  Matthew "strager" Glazar
#
# This file is part of quick-lint-js.
#
# quick-lint-js is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# quick-lint-js is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with quick-lint-js.  If not, see <https://www.gnu.org/licenses/>.
