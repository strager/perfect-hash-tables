// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_PERFECT_HASH_TABLE_H
#define PERFECT_HASH_TABLES_PERFECT_HASH_TABLE_H

#include "fnv.h"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <xxhash.h>

namespace pht {
// if (1<<0) is set: s[0]
// if (1<<1) is set: s[1]
// if (1<<2) is set: s[size-1]
// if (1<<3) is set: s[size-2]
// if (1<<4) is set: size
using character_selection_mask = std::uint8_t;

class xx3_64_hasher {
  public:
    [[gnu::always_inline]]
    explicit xx3_64_hasher(std::uint32_t basis) noexcept : seed_(basis) {}

    [[gnu::always_inline]]
    void bytes(const std::uint8_t* bytes, std::size_t length) noexcept {
        this->hash_ = ::XXH3_64bits_withSeed(bytes, length, this->seed_);
    }

    [[gnu::always_inline]]
    void bytes_4(const std::uint8_t* bytes) noexcept {
        this->bytes(bytes, 4);
    }

    std::uint32_t hash() const noexcept { return (std::uint32_t) this->hash_; }

  private:
    std::uint32_t seed_;
    std::uint64_t hash_;
};

template <class Hasher>
[[gnu::always_inline]]
inline void hash_selected_characters(character_selection_mask mask, Hasher& hasher, const char* s, std::size_t size) noexcept {
    std::array<std::uint8_t, 5> bytes;
    int i = 0;
    if (mask & (1 << 0)) bytes[i++] = (std::uint8_t)s[0];
    if (mask & (1 << 1)) bytes[i++] = (std::uint8_t)s[1];
    if (mask & (1 << 2)) bytes[i++] = (std::uint8_t)s[size - 1];
    if (mask & (1 << 3)) bytes[i++] = (std::uint8_t)s[size - 2];
    if (mask & (1 << 4)) bytes[i++] = (std::uint8_t)size;
    if (std::popcount(mask) == 4) {
        hasher.bytes_4(bytes.data());
    } else {
        hasher.bytes(bytes.data(), bytes.size());
    }
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
