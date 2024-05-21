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
#include <collie/type_safe/flag.h>

#include <collie/testing/doctest.h>

using namespace collie::ts;

TEST_CASE("flag")
{
    SUBCASE("constructor")
    {
        flag a(true);
        REQUIRE(a == true);

        flag b(false);
        REQUIRE(b == false);
    }
    SUBCASE("toggle")
    {
        flag a(true);
        REQUIRE(a.toggle());
        REQUIRE(a == false);

        flag b(false);
        REQUIRE(!b.toggle());
        REQUIRE(b == true);
    }
    SUBCASE("change")
    {
        flag a(true);
        a.change(false);
        REQUIRE(a == false);

        flag b(false);
        b.change(true);
        REQUIRE(b == true);
    }
    SUBCASE("set")
    {
        flag a(true);
        a.set();
        REQUIRE(a == true);

        flag b(false);
        b.set();
        REQUIRE(b == true);
    }
    SUBCASE("try_set")
    {
        flag a(true);
        REQUIRE(!a.try_set());
        REQUIRE(a == true);

        flag b(false);
        REQUIRE(b.try_set());
        REQUIRE(b == true);
    }
    SUBCASE("reset")
    {
        flag a(true);
        a.reset();
        REQUIRE(a == false);

        flag b(false);
        b.reset();
        REQUIRE(b == false);
    }
    SUBCASE("try_reset")
    {
        flag a(true);
        REQUIRE(a.try_reset());
        REQUIRE(a == false);

        flag b(false);
        REQUIRE(!b.try_reset());
        REQUIRE(b == false);
    }
}
