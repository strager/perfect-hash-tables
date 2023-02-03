// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "file.h"
#include "implementations.h"
#include "lex.h"
#include "token.h"
#include <benchmark/benchmark.h>
#include <string>
#include <vector>

namespace pht {
namespace {
void benchmark_look_up_identifier(::benchmark::State& state, implementation impl, const std::vector<lexed_token>& tokens) {
    for (auto _ : state) {
        for (const lexed_token& t : tokens) {
            ::benchmark::DoNotOptimize(impl.look_up_identifier(t.begin, t.size));
        }
    }
}

std::string benchmark_name(const char* test_case, implementation impl) {
    return std::string(test_case) + "/" + impl.name;
}
}
}

int main(int argc, char** argv) {
    std::vector<char> keywords_code = pht::read_file("keywords.txt");
    std::vector<pht::lexed_token> keywords_tokens = pht::lex(keywords_code.data());

    std::vector<char> non_keywords_code = pht::read_file("non-keywords.txt");
    std::vector<pht::lexed_token> non_keywords_tokens = pht::lex(non_keywords_code.data());

    std::vector<char> mixed_code = pht::read_file("mixed.txt");
    std::vector<pht::lexed_token> mixed_tokens = pht::lex(mixed_code.data());

    for (pht::implementation impl : pht::load_implementations()) {
        ::benchmark::RegisterBenchmark(pht::benchmark_name("keywords", impl).c_str(), pht::benchmark_look_up_identifier, impl, keywords_tokens);
    }
    for (pht::implementation impl : pht::load_implementations()) {
        ::benchmark::RegisterBenchmark(pht::benchmark_name("nonkeywords", impl).c_str(), pht::benchmark_look_up_identifier, impl, non_keywords_tokens);
    }
    for (pht::implementation impl : pht::load_implementations()) {
        ::benchmark::RegisterBenchmark(pht::benchmark_name("mixed", impl).c_str(), pht::benchmark_look_up_identifier, impl, mixed_tokens);
    }

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
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
