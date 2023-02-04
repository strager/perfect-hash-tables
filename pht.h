// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_PERFECT_HASH_TABLE_H
#define PERFECT_HASH_TABLES_PERFECT_HASH_TABLE_H

#include "fnv.h"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <nmmintrin.h>
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

class intel_crc32_hasher {
  public:
    [[gnu::always_inline]]
    explicit intel_crc32_hasher(std::uint32_t basis) noexcept : hash_(basis) {}

    [[gnu::always_inline]]
    void bytes(const std::uint8_t* bytes, std::size_t length) noexcept {
        // https://stackoverflow.com/a/29174491
        constexpr std::uint32_t poly = 0x82f63b78;
        for (std::size_t i = 0; i < length; ++i) {
            this->hash_ ^= bytes[i];
            for (int k = 0; k < 8; k++)
                this->hash_ = this->hash_ & 1 ? (this->hash_ >> 1) ^ poly : this->hash_ >> 1;
        }
    }

    [[gnu::always_inline]]
    void bytes_4(const std::uint8_t* bytes) noexcept {
        this->bytes(bytes, 4);
    }

    std::uint32_t hash() const noexcept { return this->hash_; }

  private:
    std::uint32_t hash_;
};

class intel_crc32_intrinsic_hasher {
  public:
    [[gnu::always_inline]]
    explicit intel_crc32_intrinsic_hasher(std::uint32_t basis) noexcept : hash_(basis) {}

    [[gnu::always_inline]]
    void bytes(const std::uint8_t* bytes, std::size_t length) noexcept {
        for (std::size_t i = 0; i < length; ++i) {
            this->hash_ = ::_mm_crc32_u8(this->hash_, bytes[i]);
        }
    }

    [[gnu::always_inline]]
    void bytes_4(const std::uint8_t* bytes) noexcept {
        std::uint32_t dword =
            std::uint32_t(bytes[0]) << 0 |
            std::uint32_t(bytes[1]) << 8 |
            std::uint32_t(bytes[2]) << 16 |
            std::uint32_t(bytes[3]) << 24;
        this->hash_ = ::_mm_crc32_u32(this->hash_, dword);
    }

    std::uint32_t hash() const noexcept { return this->hash_; }

  private:
    std::uint32_t hash_;
};

// https://en.wikipedia.org/wiki/Lehmer_random_number_generator
class lehmer_hasher {
  public:
    [[gnu::always_inline]]
    explicit lehmer_hasher(std::uint32_t basis) noexcept : hash_(basis) {}

    void bytes(const std::uint8_t* bytes, std::size_t size) noexcept {
        std::abort();
    }

    [[gnu::always_inline]]
    void bytes_4(const std::uint8_t* bytes) noexcept {
        std::uint32_t dword =
            std::uint32_t(bytes[0]) << 0 |
            std::uint32_t(bytes[1]) << 8 |
            std::uint32_t(bytes[2]) << 16 |
            std::uint32_t(bytes[3]) << 24;
        this->hash_ = mix(dword ^ this->hash_);
    }

    std::uint32_t hash() const noexcept { return this->hash_; }

  private:
    std::uint32_t hash_;

    static std::uint32_t mix(std::uint32_t seed) {
        std::uint64_t tmp;
        tmp = (std::uint64_t)seed * 0x4a39b70d;
        std::uint32_t m1 = (std::uint32_t)((tmp >> 32) ^ tmp);
        tmp = (std::uint64_t)m1 * 0x12fad5c9;
        std::uint32_t m2 = (std::uint32_t)((tmp >> 32) ^ tmp);
        return m2;
    }
};

template <class Hasher>
[[gnu::always_inline]]
inline void hash_selected_characters(character_selection_mask mask, Hasher& hasher, const char* s, std::size_t size) noexcept {
    std::array<std::uint8_t, 5> bytes;
    int i = 0;

    if ((mask & (3 << 0)) == (3 << 0)) {
        // Copy two bytes at once:
        std::memcpy(&bytes[i], s, 2);
        i += 2;
    } else {
        if (mask & (1 << 0)) bytes[i++] = (std::uint8_t)s[0];
        if (mask & (1 << 1)) bytes[i++] = (std::uint8_t)s[1];
    }

    if ((mask & (3 << 2)) == (3 << 2)) {
        // Copy two bytes at once:
        std::memcpy(&bytes[i], &s[size-2], 2);
        i += 2;
    } else {
        if (mask & (1 << 2)) bytes[i++] = (std::uint8_t)s[size - 1];
        if (mask & (1 << 3)) bytes[i++] = (std::uint8_t)s[size - 2];
    }

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
