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
#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace collie::tf {

    // Procedure: ostreamize
    template<typename T>
    void ostreamize(std::ostream &os, T &&token) {
        os << std::forward<T>(token);
    }

    // Procedure: ostreamize
    template<typename T, typename... Rest>
    void ostreamize(std::ostream &os, T &&token, Rest &&... rest) {
        os << std::forward<T>(token);
        ostreamize(os, std::forward<Rest>(rest)...);
    }

    // Function: stringify
    template<typename... ArgsT>
    std::string stringify(ArgsT &&... args) {
        std::ostringstream oss;
        ostreamize(oss, std::forward<ArgsT>(args)...);
        return oss.str();
    }


}  // namespace collie::tf
