// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_PERFECT_HASH_TABLE_H
#define PERFECT_HASH_TABLES_PERFECT_HASH_TABLE_H

#include "fnv.h"
#include <cstddef>
#include <cstdint>

namespace pht {
// if (1<<0) is set: s[0]
// if (1<<1) is set: s[1]
// if (1<<2) is set: s[size-1]
// if (1<<3) is set: s[size-2]
// if (1<<4) is set: size
using character_selection_mask = std::uint8_t;

[[gnu::always_inline]]
inline std::uint32_t hash(character_selection_mask mask, std::uint32_t hash_basis, const char* s, std::size_t size) noexcept {
    std::uint32_t h = hash_basis;
    if (mask & (1 << 0)) fnv1a32_byte(&h, (std::uint8_t)s[0]);
    if (mask & (1 << 1)) fnv1a32_byte(&h, (std::uint8_t)s[1]);
    if (mask & (1 << 2)) fnv1a32_byte(&h, (std::uint8_t)s[size - 1]);
    if (mask & (1 << 3)) fnv1a32_byte(&h, (std::uint8_t)s[size - 2]);
    if (mask & (1 << 4)) fnv1a32_byte(&h, (std::uint8_t)size);
    return h;
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
