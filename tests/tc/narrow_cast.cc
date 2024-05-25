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
#include <collie/type_safe/narrow_cast.h>

#include <collie/testing/doctest.h>

using namespace collie::ts;

TEST_CASE("narrow_cast<integer>")
{
    integer<int> a(4);

    integer<short> b = narrow_cast<short>(a);
    REQUIRE(static_cast<short>(b) == 4);

    integer<short> c = narrow_cast<integer<short>>(a);
    REQUIRE(static_cast<short>(c) == 4);

    integer<short> d = narrow_cast<integer<short>>(42);
    REQUIRE(static_cast<short>(d) == 42);
}

TEST_CASE("narrow_cast<floating_point>")
{
    floating_point<double> a(1.);

    floating_point<float> b = narrow_cast<float>(a);
    REQUIRE(static_cast<float>(b) == 1.);

    floating_point<float> c = narrow_cast<floating_point<float>>(a);
    REQUIRE(static_cast<float>(c) == 1.);
}
