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
#include <collie/type_safe/deferred_construction.h>

#include <collie/testing/doctest.h>
#include <string>

using namespace collie::ts;

TEST_CASE("deferred_construction")
{
    SUBCASE("constructor - empty")
    {
        deferred_construction<int> a;
        REQUIRE(!a.has_value());
    }
    SUBCASE("constructor - copy/move")
    {
        deferred_construction<int> a;
        a = 0;

        deferred_construction<int> b(a);
        REQUIRE(b.has_value());
        REQUIRE(b.value() == 0);

        deferred_construction<int> c(std::move(a));
        REQUIRE(c.has_value());
        REQUIRE(c.value() == 0);

        deferred_construction<int> d;

        deferred_construction<int> e(d);
        REQUIRE(!e.has_value());

        deferred_construction<int> f(std::move(d));
        REQUIRE(!d.has_value());
    }
    SUBCASE("assignment")
    {
        deferred_construction<int> a;
        REQUIRE(!a.has_value());

        a = 42;
        REQUIRE(a.has_value());
        REQUIRE(a.value() == 42);
    }
    SUBCASE("emplace")
    {
        deferred_construction<std::string> a;
        REQUIRE(!a.has_value());

        a.emplace(3u, 'c');
        REQUIRE(a.has_value());
        REQUIRE(a.value() == "ccc");
    }
    SUBCASE("operator bool")
    {
        deferred_construction<int> a;
        REQUIRE(!a);
        a = 42;
        REQUIRE(a);
    }
}
