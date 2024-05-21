//
// Copyright (C) 2024 EA group inc.
// Author: Jeff.li lijippy@163.com
// All rights reserved.
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//

#ifndef COLLIE_STRINGS_UTILITY_H_
#define COLLIE_STRINGS_UTILITY_H_

#include <string>
#include <string_view>

namespace collie {

    std::string_view safe_substring(std::string_view str, size_t pos, size_t len) {
        if (pos >= str.size()) {
            return std::string_view();
        }
        return str.substr(pos, len);
    }

    std::string_view safe_slice(std::string_view str, size_t start, size_t end) {
        if (start >= str.size() || start >= end) {
            return std::string_view();
        }
        return str.substr(start, end - start);
    }

}  // namespace collie
#endif  // COLLIE_STRINGS_UTILITY_H_
