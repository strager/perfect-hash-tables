// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_IMPLEMENTATIONS_H
#define PERFECT_HASH_TABLES_IMPLEMENTATIONS_H

#include "token.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>

namespace pht {
constexpr const char* implementation_so_paths[] = {
#include "generated/implementations.inc"
};

constexpr std::size_t implementation_count = std::size(implementation_so_paths);

struct implementation {
    const char* name;
    look_up_identifier_f* look_up_identifier;
};

inline implementation load_implementation(const char* so_path) {
    void *so = ::dlopen(so_path, /*flags=*/RTLD_LAZY);
    if (!so) {
      std::fprintf(stderr, "fatal: could not load %s: %s\n", so_path, ::dlerror());
      std::exit(1);
    }
    const char* symbol_name = "look_up_identifier";
    look_up_identifier_f* look_up_identifier = reinterpret_cast<look_up_identifier_f*>(::dlsym(so, symbol_name));
    if (!look_up_identifier) {
      std::fprintf(stderr, "fatal: could not load symbol %s in %s: %s\n", symbol_name, so_path, ::dlerror());
      std::exit(1);
    }
    return implementation{so_path, look_up_identifier};
}

inline std::array<implementation, implementation_count> load_implementations() {
    std::array<implementation, implementation_count> implementations;
    std::size_t i = 0;
    for (const char* so_path : implementation_so_paths) {
        implementations[i++] = load_implementation(so_path);
    }
    return implementations;
}
}

#endif

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
