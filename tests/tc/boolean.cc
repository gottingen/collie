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
#include <collie/type_safe/boolean.h>

#include <collie/testing/doctest.h>

#include <sstream>

using namespace collie::ts;

#ifndef TYPE_SAFE_TEST_NO_STATIC_ASSERT
static_assert(std::is_standard_layout<boolean>::value, "");
static_assert(std::is_trivially_copyable<boolean>::value, "");
// conversion checks
static_assert(std::is_constructible<boolean, bool>::value, "");
static_assert(!std::is_constructible<boolean, int>::value, "");
static_assert(std::is_assignable<boolean, bool>::value, "");
static_assert(!std::is_assignable<boolean, int>::value, "");
#endif

TEST_CASE("boolean")
{
    SUBCASE("constructor")
    {
        boolean b1(true);
        REQUIRE(static_cast<bool>(b1));

        boolean b2(false);
        REQUIRE(!static_cast<bool>(b2));
    }
    SUBCASE("assignment")
    {
        boolean b1(true);
        b1 = false;
        REQUIRE(!static_cast<bool>(b1));
        b1 = true;
        REQUIRE(static_cast<bool>(b1));
    }
    SUBCASE("negate")
    {
        boolean b1(true);
        REQUIRE(!b1 == false);

        boolean b2(false);
        REQUIRE(!b2 == true);
    }
    SUBCASE("comparison")
    {
        boolean b1(true);
        REQUIRE(b1 == true);
        REQUIRE(true == b1);
        REQUIRE(b1 != false);
        REQUIRE(false != b1);
        REQUIRE(b1 == boolean(true));
        REQUIRE(b1 != boolean(false));

        boolean b2(false);
        REQUIRE(b2 == false);
        REQUIRE(false == b2);
        REQUIRE(b2 != true);
        REQUIRE(true != b2);
        REQUIRE(b2 == boolean(false));
        REQUIRE(b2 != boolean(true));
    }
    SUBCASE("i/o")
    {
        std::ostringstream out;
        std::istringstream in("0");

        boolean b(true);
        out << b;
        REQUIRE(out.str() == "1");

        in >> b;
        REQUIRE(!static_cast<bool>(b));
    }
}
