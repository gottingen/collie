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
#include <collie/type_safe/index.h>

#include <collie/testing/doctest.h>

using collie::ts::difference_t;
using collie::ts::index_t;

TEST_CASE("index_t")
{
    index_t idx;
    REQUIRE(idx == index_t(0u));

    SUBCASE("operator+=")
    {
        difference_t a(5);
        idx += a;
        REQUIRE(idx == index_t(5u));

        difference_t b(-3);
        idx += b;
        REQUIRE(idx == index_t(2u));
    }
    SUBCASE("operator-=")
    {
        idx = index_t(10u);

        difference_t a(5);
        idx -= a;
        REQUIRE(idx == index_t(5u));

        difference_t b(-3);
        idx -= b;
        REQUIRE(idx == index_t(8u));
    }
    SUBCASE("operator+")
    {
        auto c = idx + difference_t(5);
        REQUIRE(c == index_t(5u));

        auto d = difference_t(5) + idx;
        REQUIRE(d == index_t(5u));

        auto e = c + difference_t(-3);
        REQUIRE(e == index_t(2u));

        auto f = difference_t(-3) + d;
        REQUIRE(f == index_t(2u));
    }
    SUBCASE("next")
    {
        auto a = next(idx, difference_t(5));
        REQUIRE(a == index_t(5u));

        auto b = next(a, difference_t(-3));
        REQUIRE(b == index_t(2u));
    }
    SUBCASE("prev")
    {
        idx = index_t(10u);

        auto a = prev(idx, difference_t(5));
        REQUIRE(a == index_t(5u));

        auto b = prev(a, difference_t(-3));
        REQUIRE(b == index_t(8u));
    }
    SUBCASE("advance")
    {
        advance(idx, difference_t(5));
        REQUIRE(idx == index_t(5u));

        advance(idx, difference_t(-3));
        REQUIRE(idx == index_t(2u));
    }
    SUBCASE("operator-")
    {
        idx = index_t(10u);

        auto c = idx - difference_t(5);
        REQUIRE(c == index_t(5u));

        auto d = c - difference_t(-3);
        REQUIRE(d == index_t(8u));
    }
    SUBCASE("distance")
    {
        auto a = index_t(5u) - idx;
        REQUIRE(a == difference_t(5));
        REQUIRE(a == distance(idx, index_t(5u)));

        auto b = idx - index_t(5u);
        REQUIRE(b == difference_t(-5));
        REQUIRE(b == distance(index_t(5u), idx));
    }
    SUBCASE("at")
    {
        std::size_t array[] = {0, 1, 2, 3, 4, 5};

        for (index_t i; i != 5u; ++i)
            REQUIRE(at(array, i) == std::size_t(get(i)));
    }
}
