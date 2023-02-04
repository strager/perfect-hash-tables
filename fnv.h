// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#ifndef PERFECT_HASH_TABLES_FNV_H
#define PERFECT_HASH_TABLES_FNV_H

#include <cstdint>

namespace pht {
class fnv1a32 {
    public:
    [[gnu::always_inline]]
    explicit fnv1a32(std::uint32_t basis) noexcept : hash_(basis) {}

    [[gnu::always_inline]]
    void byte(std::uint8_t byte) noexcept {
        this->hash_ = this->hash_ ^ byte;
        this->hash_ = this->hash_ * 0x01000193;
    }

    std::uint32_t hash() const noexcept { return this->hash_; }

    private:
    std::uint32_t hash_;
};
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
