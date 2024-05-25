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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <collie/testing/doctest.h>
#include <collie/container/iterator.h>

// --------------------------------------------------------
// Testcase: distance
// --------------------------------------------------------
TEST_CASE("distance.integral" * doctest::timeout(300)) {

    auto count = [](int beg, int end, int step) {
        size_t c = 0;
        for (int i = beg; step > 0 ? i < end : i > end; i += step) {
            ++c;
        }
        return c;
    };

    for (int beg = -50; beg <= 50; ++beg) {
        for (int end = -50; end <= 50; ++end) {
            if (beg < end) {   // positive step
                for (int s = 1; s <= 50; s++) {
                    REQUIRE((collie::distance(beg, end, s) == count(beg, end, s)));
                }
            } else {            // negative step
                for (int s = -1; s >= -50; s--) {
                    REQUIRE((collie::distance(beg, end, s) == count(beg, end, s)));
                }
            }
        }
    }

}