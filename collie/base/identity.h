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
// Created by jeff on 24-3-27.
//

#ifndef COLLIE_BASE_IDENTITY_H_
#define COLLIE_BASE_IDENTITY_H_

namespace collie {

    // Similar to `std::identity` from C++20.
    template <class Ty> struct identity {
        using is_transparent = void;
        using argument_type = Ty;

        Ty &operator()(Ty &self) const {
            return self;
        }
        const Ty &operator()(const Ty &self) const {
            return self;
        }
    };

}  // namespace collie

#endif  // COLLIE_BASE_IDENTITY_H_
