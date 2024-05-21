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

using namespace collie;

int main() {
    constexpr std::string_view r1 = ">=1.2.7 <1.3.0";
    static_assert(module_range::satisfies("1.2.7"_version, r1));
    static_assert(module_range::satisfies("1.2.8"_version, r1));
    static_assert(module_range::satisfies("1.2.99"_version, r1));
    static_assert(!module_range::satisfies("1.2.6"_version, r1));
    static_assert(!module_range::satisfies("1.3.0"_version, r1));
    static_assert(!module_range::satisfies("1.1.0"_version, r1));

    constexpr std::string_view r2 = "1.2.7 || >=1.2.9 <2.0.0";
    static_assert(module_range::satisfies(ModuleVersion{1, 2, 7}, r2));
    static_assert(module_range::satisfies({1, 2, 9}, r2));
    static_assert(!module_range::satisfies("1.2.8"_version, r2));
    static_assert(!module_range::satisfies("2.0.0"_version, r2));

    // By default, we exclude prerelease tag from comparison.
    constexpr std::string_view r3 = ">1.2.3-alpha.3";
    static_assert(module_range::satisfies("1.2.3-alpha.7"_version, r3));
    static_assert(!module_range::satisfies("3.4.5-alpha.9"_version, r3));

    static_assert(module_range::satisfies("3.4.5-alpha.9"_version, r3, module_range::satisfies_option::include_prerelease));

    return 0;
}
