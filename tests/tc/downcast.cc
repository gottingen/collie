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
#include <collie/type_safe/downcast.h>

#include <collie/testing/doctest.h>

using namespace collie::ts;

TEST_CASE("downcast")
{
    struct base
    {
        virtual ~base() = default;
    };

    struct derived : base
    {
        ~derived() override = default;
    };

    base    b;
    derived d;

    SUBCASE("base -> base")
    {
        base& ref = b;

        base& res1 = downcast<base&>(ref);
        REQUIRE(&res1 == &ref);

        base& res2 = downcast(derived_type<base>{}, ref);
        REQUIRE(&res2 == &ref);
    }
    SUBCASE("const base -> const base")
    {
        const base& ref = b;

        const base& res1 = downcast<const base&>(ref);
        REQUIRE(&res1 == &ref);

        const base& res2 = downcast(derived_type<base>{}, ref);
        REQUIRE(&res2 == &ref);
    }
    SUBCASE("base -> derived")
    {
        base& ref = d;

        derived& res1 = downcast<derived&>(ref);
        REQUIRE(&res1 == &ref);

        derived& res2 = downcast(derived_type<derived>{}, ref);
        REQUIRE(&res2 == &ref);
    }
    SUBCASE("const base -> const derived")
    {
        const base& ref = d;

        const derived& res1 = downcast<const derived&>(ref);
        REQUIRE(&res1 == &ref);

        const derived& res2 = downcast(derived_type<derived>{}, ref);
        REQUIRE(&res2 == &ref);
    }
}
