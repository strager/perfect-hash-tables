// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include "token.h"
#include <array>
#include <frozen/string.h>
#include <frozen/unordered_map.h>

namespace pht {
namespace {
constexpr ::frozen::unordered_map<::frozen::string, token_type, std::size(keyword_tokens)> keywords = {
    PHT_KEYWORD_TOKENS_TABLE
};
}

extern "C" 
token_type look_up_identifier(const char* identifier, std::size_t size) noexcept {
    auto it = keywords.find(std::string_view(identifier, size));
    if (it == keywords.end()) {
        return token_type::identifier;
    }
    return it->second;
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
