// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "file.h"
#include "implementations.h"
#include "lex.h"
#include "token.h"
#include <benchmark/benchmark.h>

namespace pht {
namespace {
const std::array<implementation, implementation_count>& lazy_load_implementations() {
    static std::array<implementation, implementation_count> implementations
       = load_implementations();
    return implementations;
}

void add_implementations_to_benchmark(::benchmark::internal::Benchmark* b) {
    int index = 0;
    for (implementation _impl : lazy_load_implementations()) {
        b->Arg(index++);
    }
}

void non_keywords(::benchmark::State& state) {
    implementation impl = lazy_load_implementations().at(state.range(0));
    std::vector<char> code = read_file("non-keywords.txt");
    std::vector<lexed_token> tokens = lex(code.data());
    for (auto _ : state) {
        for (const lexed_token& t : tokens) {
            ::benchmark::DoNotOptimize(impl.look_up_identifier(t.begin, t.size));
        }
    }
}
BENCHMARK(non_keywords)->Apply(add_implementations_to_benchmark);

void keywords(::benchmark::State& state) {
    implementation impl = lazy_load_implementations().at(state.range(0));
    std::vector<char> code = read_file("keywords.txt");
    std::vector<lexed_token> tokens = lex(code.data());
    for (auto _ : state) {
        for (const lexed_token& t : tokens) {
            ::benchmark::DoNotOptimize(impl.look_up_identifier(t.begin, t.size));
        }
    }
}
BENCHMARK(keywords)->Apply(add_implementations_to_benchmark);

void mixed(::benchmark::State& state) {
    implementation impl = lazy_load_implementations().at(state.range(0));
    std::vector<char> code = read_file("mixed.txt");
    std::vector<lexed_token> tokens = lex(code.data());
    for (auto _ : state) {
        for (const lexed_token& t : tokens) {
            ::benchmark::DoNotOptimize(impl.look_up_identifier(t.begin, t.size));
        }
    }
}
BENCHMARK(mixed)->Apply(add_implementations_to_benchmark);
}
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
