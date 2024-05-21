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
#include <collie/type_safe/constrained_type.h>

#include <collie/testing/doctest.h>

using namespace collie::ts;

struct test_verifier
{
    static bool expected;

    template <typename T, typename Predicate>
    static T&& verify(T&& value, const Predicate& p)
    {
        REQUIRE(p(value) == expected);
        return std::forward<T>(value);
    }
};

bool test_verifier::expected;

struct test_predicate
{
    bool operator()(int i) const
    {
        return i != -1;
    }
};

TEST_CASE("constrained_type")
{
    using my_int = constrained_type<int, test_predicate, test_verifier>;

    SUBCASE("constructor")
    {
        test_verifier::expected = true;
        my_int a(5);
        REQUIRE(a.get_value() == 5);
        my_int b(-4);
        REQUIRE(b.get_value() == -4);

        test_verifier::expected = false;
        my_int c(-1);
        REQUIRE(c.get_value() == -1);
    }
    SUBCASE("assignment")
    {
        test_verifier::expected = true;
        my_int a(5);
        a = 2;
        a = 3;
        a = -2;

        test_verifier::expected = false;
        a                       = -1;
    }
    SUBCASE("constrain")
    {
        test_verifier::expected = true;
        my_int a                = constrain<test_verifier>(5, test_predicate{});
        REQUIRE(a.get_value() == 5);
        my_int b = constrain<test_verifier>(-4, test_predicate{});
        REQUIRE(b.get_value() == -4);

        test_verifier::expected = false;
        my_int c                = constrain<test_verifier>(-1, test_predicate{});
        REQUIRE(c.get_value() == -1);
    }
    SUBCASE("modify()")
    {
        // with() is the same
        test_verifier::expected = true;
        my_int a(4);
        {
            auto modify = a.modify();
            modify.get() += 4;
        }
        REQUIRE(a.get_value() == 8);
        {
            auto modify = a.modify();
            modify.get() -= 5;
            modify.get() = 2;
        }
        REQUIRE(a.get_value() == 2);

        {
            auto modify             = a.modify();
            modify.get()            = -1;
            test_verifier::expected = false;
        }
        REQUIRE(a.get_value() == -1);
        try
        {
            auto modify  = a.modify();
            modify.get() = -1;
            throw 0;
        }
        catch (...)
        {
            REQUIRE(a.get_value() == -1);
        }
    }
}

TEST_CASE("constrained_ref")
{
    using my_ref = constrained_ref<int, test_predicate, test_verifier>;

    auto valid1  = 5;
    auto valid2  = -4;
    auto invalid = -1;

    SUBCASE("constructor")
    {
        test_verifier::expected = true;
        my_ref a(valid1);
        REQUIRE(a.get_value() == 5);
        my_ref b(valid2);
        REQUIRE(b.get_value() == -4);

        test_verifier::expected = false;
        my_ref c(invalid);
        REQUIRE(c.get_value() == -1);
    }
    SUBCASE("modify()")
    {
        // with() is the same
        test_verifier::expected = true;
        my_ref a(valid1);
        {
            auto modify = a.modify();
            modify.get() += 3;
        }
        REQUIRE(a.get_value() == 8);
        {
            auto modify = a.modify();
            modify.get() -= 5;
            modify.get() = 2;
        }
        REQUIRE(a.get_value() == 2);

        {
            auto modify             = a.modify();
            modify.get()            = -1;
            test_verifier::expected = false;
        }
        REQUIRE(a.get_value() == -1);
        try
        {
            auto modify  = a.modify();
            modify.get() = -1;
            throw 0;
        }
        catch (...)
        {
            REQUIRE(a.get_value() == -1);
        }
    }
}

TEST_CASE("throwing_verifier")
{
    auto dummy1 = 0, dummy2 = 0;

    constrained_type<int*, constraints::non_null, throwing_verifier> a(&dummy1);
    REQUIRE_NOTHROW((a = &dummy2, true));
    REQUIRE_THROWS_AS(a = static_cast<int*>(nullptr), constrain_error);

    constrained_type<int*, constraints::non_null, throwing_verifier> b
        = sanitize(&dummy2, constraints::non_null{});
    REQUIRE_NOTHROW((b = &dummy2, true));
    REQUIRE_THROWS_AS(b = static_cast<int*>(nullptr), constrain_error);
}

TEST_CASE("constraints::non_null")
{
#ifndef TYPE_SAFE_TEST_NO_STATIC_ASSERT
    // conversion checks
    using ptr = constrained_type<int*, constraints::non_null>;
    static_assert(std::is_constructible<ptr, int*>::value, "");
    static_assert(!std::is_constructible<ptr, std::nullptr_t>::value, "");
    static_assert(std::is_assignable<ptr, int*>::value, "");
    static_assert(!std::is_assignable<ptr, std::nullptr_t>::value, "");
#endif

    constraints::non_null p;
    REQUIRE(!p(static_cast<int*>(nullptr)));
    REQUIRE(!p(static_cast<int*>(0)));

    int a;
    REQUIRE(p(&a));
}

struct my_container
{
    bool empty;
};

bool empty(my_container c)
{
    return c.empty;
}

TEST_CASE("constraints::non_empty")
{
    constraints::non_empty p;
    REQUIRE(p(std::string("hi")));
    REQUIRE(!p(std::string()));

    REQUIRE(p(my_container{false}));
    REQUIRE(!p(my_container{true}));
}

TEST_CASE("constraints::non_default")
{
    constraints::non_default p;
    REQUIRE(p(5));
    REQUIRE(p(-1));
    REQUIRE(!p(int()));
}

TEST_CASE("constraints::non_invalid")
{
    constraints::non_invalid p;

    REQUIRE(p(&p));
    REQUIRE(!p(static_cast<void*>(nullptr)));

    REQUIRE(!p(false));
    REQUIRE(p(true));

    struct my_bool
    {
        bool b;

        explicit operator bool() const noexcept
        {
            return b;
        }
    };
    REQUIRE(p(my_bool{true}));
    REQUIRE(!p(my_bool{false}));
}
