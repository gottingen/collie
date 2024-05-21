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

namespace collie {

    /** @class Singleton

    @brief class template to create a thread-safe singleton object

    */
    template<typename T>
    class Singleton {

    public:

        /**
        @brief get a reference to the singleton object
        */
        inline static T &get() {
            static T instance;
            return instance;
        }

    private:

        Singleton() = default;

        ~Singleton() = default;

        Singleton(const Singleton &) = delete;

        Singleton &operator=(const Singleton &) = delete;
    };


}  // namespace collie
