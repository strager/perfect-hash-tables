// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "implementations.h"
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

void failures(::benchmark::State& state) {
    implementation impl = lazy_load_implementations().at(state.range(0));
    for (auto _ : state) {
        ::benchmark::DoNotOptimize(impl.look_up_identifier("banana", 6));
    }
}
BENCHMARK(failures)->Apply(add_implementations_to_benchmark);
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
