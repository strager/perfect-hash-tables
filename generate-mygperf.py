# Copyright (C) 2020  Matthew "strager" Glazar
# See end of file for extended copyright information.

import typing
import argparse
import pathlib
import subprocess

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", required=True, metavar="PATH")
    args = parser.parse_args()

    output_path = pathlib.Path(args.output)

    raw_gperf_output = subprocess.check_output(["gperf", "token.gperf"], encoding="utf-8")
    tables = parse_gperf_output(raw_gperf_output)

    with open(output_path, "w") as out:
        out.write(f"""\
#include <stddef.h>
#include <string.h>
#include "token.h"

namespace pht {{
namespace {{
""")
        out.write(tables.constants.replace("{", "constexpr int ").replace("}", "").replace(",", ";\nconstexpr int "))
        out.write(f"""\
struct keyword_entry {{
  char string[MAX_WORD_LENGTH + 1];
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

    const char *s = wordlist[h].string;

    if (!strncmp(str, s, len) && s[len] == '\\0') {{
      return wordlist[h].type;
    }}

    return token_type::identifier;
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
