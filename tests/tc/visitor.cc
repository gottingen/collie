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
#include <collie/type_safe/visitor.h>

#include <collie/testing/doctest.h>

using collie::ts::nullopt_t;
using collie::ts::nullvar_t;
using collie::ts::optional;
using collie::ts::variant;

TEST_CASE("visit optional")
{
    struct visitor
    {
        using incomplete_visitor = void;

        int value;

        void operator()(nullopt_t) const
        {
            REQUIRE(value == -1);
        }

        void operator()(int i) const
        {
            REQUIRE(value == i);
        }

        void operator()(int, nullopt_t) const
        {
            REQUIRE(value == -1);
        }

        void operator()(int, int b) const
        {
            REQUIRE(value == b);
        }
    };

    optional<int> a;
    visit(visitor{-1}, a);

    a = 42;
    visit(visitor{42}, a);

    optional<int> b;
    visit(visitor{-1}, a, b);

    b = 32;
    visit(visitor{32}, a, b);
}

TEST_CASE("visit variant")
{
    SUBCASE("optional variant")
    {
        struct visitor
        {
            int value;

            void operator()(nullvar_t) const
            {
                REQUIRE(value == -1);
            }

            void operator()(int i) const
            {
                REQUIRE(value == i);
            }

            void operator()(float f) const
            {
                REQUIRE(f == 3.14f);
            }

            void operator()(nullvar_t, nullvar_t) const
            {
                REQUIRE(false);
            }
            void operator()(nullvar_t, int) const
            {
                REQUIRE(false);
            }

            void operator()(int, nullvar_t) const
            {
                REQUIRE(value == -1);
            }
            void operator()(int, int b) const
            {
                REQUIRE(value == b);
            }

            void operator()(float, nullvar_t) const
            {
                REQUIRE(false);
            }
            void operator()(float a, int b) const
            {
                REQUIRE(value == b);
                REQUIRE(a == 3.14f);
            }
        };

        variant<nullvar_t, int, float> a;
        visit(visitor{-1}, a);

        a = 42;
        visit(visitor{42}, a);

        variant<nullvar_t, int> b;
        visit(visitor{-1}, a, b);

        b = 32;
        visit(visitor{32}, a, b);

        a = 3.14f;
        visit(visitor{-1}, a);
        visit(visitor{32}, a, b);
    }
    SUBCASE("returning reference")
    {
        struct visitor
        {
            int  value;
            int& operator()(int)
            {
                return value;
            }
        };

        visitor      v{1};
        variant<int> x{1};
        visit(v, x) = 2;

        REQUIRE(v.value == 2);
    }
    SUBCASE("rarely empty variant")
    {
        struct visitor
        {
            int value;

            void operator()(int a)
            {
                REQUIRE(a == value);
            }

            void operator()(float b)
            {
                REQUIRE(b == 3.14f);
                REQUIRE(value == -1);
            }

            void operator()(int, int)
            {
                REQUIRE(false);
            }

            void operator()(int, float)
            {
                REQUIRE(false);
            }

            void operator()(float a, int b)
            {
                REQUIRE(a == 3.14f);
                REQUIRE(b == value);
            }

            void operator()(float, float)
            {
                REQUIRE(false);
            }
        };

        variant<int, float> a(0);
        visit(visitor{0}, a);

        a = 3.14f;
        visit(visitor{-1}, a);

        variant<int, float> b(0);
        visit(visitor{0}, a, b);
    }
    SUBCASE("return type")
    {
        variant<int> a(1);
        auto         b = a;

        REQUIRE(visit([](int) { return 0; }, a) == 0);
        REQUIRE(visit([](int, int) { return 0; }, a, b) == 0);
    }
}
