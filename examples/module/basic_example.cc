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


#include <collie/module/module.h>

#include <iostream>

using namespace collie;

int main() {
    constexpr ModuleVersion v_default;
    static_assert(v_default == ModuleVersion(0, 1, 0, prerelease::none, 0));
    std::cout << v_default << std::endl; // 0.1.0

    constexpr ModuleVersion v1{1, 4, 3};
    constexpr ModuleVersion v2{"1.2.4-alpha.10"};
    std::cout << v1 << std::endl; // 1.4.3
    std::cout << v2 << std::endl; // 1.2.4-alpha.10
    static_assert(v1 != v2);
    static_assert(!(v1 == v2));
    static_assert(v1 > v2);
    static_assert(v1 >= v2);
    static_assert(!(v1 < v2));
    static_assert(!(v1 <= v2));

    ModuleVersion v_s;
    v_s.from_string("1.2.3-rc.1");
    std::string s1 = v_s.to_string();
    std::cout << s1 << std::endl; // 1.2.3-rc.1
    v_s.prerelease_number = 0;
    std::string s2 = v_s.to_string();
    std::cout << s2 << std::endl; // 1.2.3-rc

    constexpr ModuleVersion vo = "2.0.0-rc.3"_version;
    std::cout << vo << std::endl; // 2.0.0-rc.3

    return 0;
}
