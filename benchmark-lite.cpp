// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "file.h"
#include "implementations.h"
#include "lex.h"
#include "token.h"
#include <benchmark/benchmark.h>
#include <cstring>
#include <string>
#include <vector>

namespace pht {
namespace {
void go(const char* so_path) {
    implementation impl = load_implementation(so_path);
    std::vector<char> code = read_file("mixed.txt");
    std::vector<pht::lexed_token> tokens = lex(code.data());
    for (int i = 0; i < 10'000; ++i) {
        for (const lexed_token& t : tokens) {
            ::benchmark::DoNotOptimize(impl.look_up_identifier(t.begin, t.size));
        }
    }
}
}
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        std::fprintf(stderr, "error: expected .so path\n");
        std::exit(1);
    }
    const char* so_path = argv[1];
    pht::go(so_path);
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
