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

#include <collie/type_safe/integer.h>

#include <collie/testing/doctest.h>

#include <climits>
#include <sstream>

using namespace collie::ts;

#ifndef TYPE_SAFE_TEST_NO_STATIC_ASSERT
static_assert(std::is_standard_layout<integer<int>>::value, "");
static_assert(std::is_trivially_copyable<integer<int>>::value, "");
// conversion checks
static_assert(sizeof(short) < sizeof(int), "");
static_assert(std::is_constructible<integer<short>, short>::value, "");
static_assert(!std::is_constructible<integer<short>, int>::value, "");
static_assert(!std::is_constructible<integer<short>, unsigned short>::value, "");
static_assert(!std::is_constructible<integer<unsigned short>, short>::value, "");

static_assert(std::is_assignable<integer<short>, short>::value, "");
static_assert(!std::is_assignable<integer<short>, int>::value, "");
static_assert(!std::is_assignable<integer<short>, unsigned short>::value, "");
static_assert(!std::is_assignable<integer<unsigned short>, short>::value, "");

static_assert(sizeof(int) < sizeof(long long), "");
static_assert(std::is_constructible<integer<int>, int>::value, "");
static_assert(std::is_constructible<integer<int>, short>::value, "");
static_assert(!std::is_constructible<integer<int>, long long>::value, "");
static_assert(!std::is_constructible<integer<int>, unsigned>::value, "");
static_assert(!std::is_constructible<integer<unsigned>, int>::value, "");

static_assert(std::is_assignable<integer<int>, int>::value, "");
static_assert(std::is_assignable<integer<int>, short>::value, "");
static_assert(!std::is_assignable<integer<int>, long long>::value, "");
static_assert(!std::is_assignable<integer<int>, unsigned>::value, "");
static_assert(!std::is_assignable<integer<unsigned>, int>::value, "");
#endif

TEST_CASE("integer")
{
    using int_t = integer<int>;
    using uint_t = integer<unsigned int>;

    SUBCASE("constructor")
    {
        int_t a(0);
        REQUIRE(static_cast<int>(a) == 0);
        int_t b(32);
        REQUIRE(static_cast<int>(b) == 32);
        int_t c(-25);
        REQUIRE(static_cast<int>(c) == -25);
    }
    SUBCASE("assignment")
    {
        int_t a(0);
        a = 32;
        REQUIRE(static_cast<int>(a) == 32);
        a = -25;
        REQUIRE(static_cast<int>(a) == -25);
    }
    SUBCASE("unary")
    {
        int_t a(13);
        REQUIRE(static_cast<int>(+a) == static_cast<int>(a));
        REQUIRE(static_cast<int>(-a) == -static_cast<int>(a));
    }
    SUBCASE("increment")
    {
        int_t a(0);
        REQUIRE(static_cast<int>(++a) == 1);
        REQUIRE(static_cast<int>(a++) == 1);
        REQUIRE(static_cast<int>(a) == 2);
    }
    SUBCASE("decrement")
    {
        int_t a(0);
        REQUIRE(static_cast<int>(--a) == -1);
        REQUIRE(static_cast<int>(a--) == -1);
        REQUIRE(static_cast<int>(a) == -2);
    }
    SUBCASE("addition")
    {
        int_t wrapper(0);
        int   normal(0);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper += 5;
        normal += 5;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper += short(5);
        normal += short(5);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = wrapper + (-23);
        normal  = normal + (-23);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = 22 + wrapper;
        normal  = 22 + normal;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = int_t(-4) + wrapper;
        normal  = (-4) + normal;
        REQUIRE(static_cast<int>(wrapper) == normal);
    }
    SUBCASE("subtraction")
    {
        int_t wrapper(0);
        int   normal(0);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper -= 5;
        normal -= 5;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper -= short(5);
        normal -= short(5);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = wrapper - (-23);
        normal  = normal - (-23);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = 22 - wrapper;
        normal  = 22 - normal;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = int_t(-4) - wrapper;
        normal  = (-4) - normal;
        REQUIRE(static_cast<int>(wrapper) == normal);
    }
    SUBCASE("multiplication")
    {
        int_t wrapper(1);
        int   normal(1);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper *= 5;
        normal *= 5;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper *= short(5);
        normal *= short(5);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = wrapper * (-23);
        normal  = normal * (-23);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = 22 * wrapper;
        normal  = 22 * normal;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = int_t(-4) * wrapper;
        normal  = (-4) * normal;
        REQUIRE(static_cast<int>(wrapper) == normal);
    }
    SUBCASE("division")
    {
        int_t wrapper(23 * 25);
        int   normal(23 * 25);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper /= 5;
        normal /= 5;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper /= short(5);
        normal /= short(5);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = wrapper / (-23);
        normal  = normal / (-23);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = 22 / wrapper;
        normal  = 22 / normal;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = int_t(-4) / wrapper;
        normal  = (-4) / normal;
        REQUIRE(static_cast<int>(wrapper) == normal);
    }
    SUBCASE("modulo")
    {
        int_t wrapper(24 * 6);
        int   normal(24 * 6);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper %= 5;
        normal %= 5;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper %= short(5);
        normal %= short(5);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = wrapper % (-23);
        normal  = normal % (-23);
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = 22 % wrapper;
        normal  = 22 % normal;
        REQUIRE(static_cast<int>(wrapper) == normal);

        wrapper = int_t(-4) % wrapper;
        normal  = (-4) % normal;
        REQUIRE(static_cast<int>(wrapper) == normal);
    }
    SUBCASE("comparison")
    {
        unsigned int u32_max = std::numeric_limits<unsigned int>::max();

        // ==
        REQUIRE(bool(int_t(4) == int_t(4)));
        REQUIRE(!(int_t(5) == int_t(4)));
        REQUIRE(bool(uint_t(4u) == int_t(4)));
        REQUIRE(!(uint_t(5u) == int_t(4)));
        REQUIRE(!(uint_t(u32_max) == int_t(-1)));
        REQUIRE(bool(int_t(4) == uint_t(4u)));
        REQUIRE(!(int_t(5) == uint_t(4u)));
        REQUIRE(!(int_t(-1) == uint_t(u32_max)));

        REQUIRE(bool(4 == int_t(4)));
        REQUIRE(!(5 == int_t(4)));
        REQUIRE(bool(4u == int_t(4)));
        REQUIRE(!(5u == int_t(4)));
        REQUIRE(!(u32_max == int_t(-1)));
        REQUIRE(bool(4 == uint_t(4u)));
        REQUIRE(!(5 == uint_t(4u)));
        REQUIRE(!(-1 == uint_t(u32_max)));

        REQUIRE(bool(int_t(4) == 4));
        REQUIRE(!(int_t(5) == 4));
        REQUIRE(bool(int_t(4) == 4u));
        REQUIRE(!(int_t(5) == 4u));
        REQUIRE(!(int_t(-1) == u32_max));
        REQUIRE(bool(uint_t(4u) == 4));
        REQUIRE(!(uint_t(5u) == 4));
        REQUIRE(!(uint_t(u32_max) == -1));

        // !=
        REQUIRE(bool(int_t(5) != int_t(4)));
        REQUIRE(!(int_t(4) != int_t(4)));
        REQUIRE(bool(uint_t(5u) != int_t(4)));
        REQUIRE(!(uint_t(4u) != int_t(4)));
        REQUIRE(bool(uint_t(u32_max) != int_t(-1)));
        REQUIRE(bool(int_t(5) != uint_t(4u)));
        REQUIRE(!(int_t(4) != uint_t(4u)));
        REQUIRE(bool(int_t(-1) != uint_t(u32_max)));

        REQUIRE(bool(5 != int_t(4)));
        REQUIRE(!(4 != int_t(4)));
        REQUIRE(bool(5u != int_t(4)));
        REQUIRE(!(4u != int_t(4)));
        REQUIRE(bool(u32_max != int_t(-1)));
        REQUIRE(bool(5 != uint_t(4u)));
        REQUIRE(!(4 != uint_t(4u)));
        REQUIRE(bool(-1 != uint_t(u32_max)));

        REQUIRE(bool(int_t(5) != 4));
        REQUIRE(!(int_t(4) != 4));
        REQUIRE(bool(int_t(5) != 4u));
        REQUIRE(!(int_t(4) != 4u));
        REQUIRE(bool(int_t(-1) != u32_max));
        REQUIRE(bool(uint_t(5u) != 4));
        REQUIRE(!(uint_t(4u) != 4));
        REQUIRE(bool(uint_t(u32_max) != -1));

        // <
        REQUIRE(bool(int_t(4) < int_t(5)));
        REQUIRE(!(int_t(5) < int_t(4)));
        REQUIRE(!(int_t(4) < int_t(4)));
        REQUIRE(bool(uint_t(4u) < int_t(5)));
        REQUIRE(!bool(uint_t(4u) < int_t(-5)));
        REQUIRE(!(uint_t(5u) < int_t(4)));
        REQUIRE(!(uint_t(4u) < int_t(4)));
        REQUIRE(bool(int_t(4) < uint_t(5u)));
        REQUIRE(bool(int_t(-4) < uint_t(5u)));
        REQUIRE(!(int_t(5) < uint_t(4u)));
        REQUIRE(!(int_t(4) < uint_t(4u)));

        REQUIRE(bool(4 < int_t(5)));
        REQUIRE(!(5 < int_t(4)));
        REQUIRE(!(4 < int_t(4)));
        REQUIRE(bool(4u < int_t(5)));
        REQUIRE(!bool(4u < int_t(-5)));
        REQUIRE(!(5u < int_t(4)));
        REQUIRE(!(4u < int_t(4)));
        REQUIRE(bool(4 < uint_t(5u)));
        REQUIRE(bool(-4 < uint_t(5u)));
        REQUIRE(!(5 < uint_t(4u)));
        REQUIRE(!(4 < uint_t(4u)));

        REQUIRE(bool(int_t(4) < 5));
        REQUIRE(!(int_t(5) < 4));
        REQUIRE(!(int_t(4) < 4));
        REQUIRE(bool(int_t(4) < 5u));
        REQUIRE(!(int_t(5) < 4u));
        REQUIRE((int_t(-5) < 4u));
        REQUIRE(!(int_t(4) < 4u));
        REQUIRE(bool(uint_t(4u) < 5));
        REQUIRE(!bool(uint_t(4u) < -5));
        REQUIRE(!(uint_t(5u) < 4));
        REQUIRE(!(uint_t(4u) < 4));

        // <=
        REQUIRE(bool(int_t(4) <= int_t(5)));
        REQUIRE(!(int_t(5) <= int_t(4)));
        REQUIRE(bool(int_t(4) <= int_t(4)));
        REQUIRE(bool(uint_t(4u) <= int_t(5)));
        REQUIRE(!(uint_t(4u) <= int_t(-5)));
        REQUIRE(!(uint_t(5u) <= int_t(4)));
        REQUIRE(bool(uint_t(4u) <= int_t(4)));
        REQUIRE(bool(int_t(4) <= uint_t(5u)));
        REQUIRE(!(int_t(5) <= uint_t(4u)));
        REQUIRE(bool(int_t(-5) <= uint_t(4u)));
        REQUIRE(bool(int_t(4) <= uint_t(4u)));

        REQUIRE(bool(4 <= int_t(5)));
        REQUIRE(!(5 <= int_t(4)));
        REQUIRE(bool(4 <= int_t(4)));
        REQUIRE(bool(4u <= int_t(5)));
        REQUIRE(!(5u <= int_t(4)));
        REQUIRE(bool(4u <= int_t(4)));
        REQUIRE(bool(4 <= int_t(5)));
        REQUIRE(!(5 <= int_t(4)));
        REQUIRE(bool(4 <= int_t(4)));
        REQUIRE(!(4 <= int_t(-4)));
        REQUIRE(bool(4u <= int_t(5)));
        REQUIRE(!(5u <= int_t(4)));
        REQUIRE(bool(4u <= int_t(4)));
        REQUIRE(bool(4 <= uint_t(5u)));
        REQUIRE(!(5 <= uint_t(4u)));
        REQUIRE(bool(4 <= uint_t(4u)));
        REQUIRE(bool(-4 <= uint_t(4u)));

        REQUIRE(bool(int_t(4) <= 5));
        REQUIRE(!(int_t(5) <= 4));
        REQUIRE(bool(int_t(4) <= 4));
        REQUIRE(bool(int_t(-4) <= 4));
        REQUIRE(bool(int_t(4) <= 5u));
        REQUIRE(!(int_t(5) <= 4u));
        REQUIRE(bool(int_t(4) <= 4u));
        REQUIRE((int_t(4) <= 5));
        REQUIRE(!(int_t(5) <= 4));
        REQUIRE(bool(int_t(4) <= 4));
        REQUIRE(bool(uint_t(4u) <= 5));
        REQUIRE(!(uint_t(5u) <= 4));
        REQUIRE(bool(uint_t(4u) <= 4));
        REQUIRE(!(uint_t(4u) <= -4));
        REQUIRE(bool(int_t(4) <= 5u));
        REQUIRE(!(int_t(5) <= 4u));
        REQUIRE(bool(int_t(4) <= 4u));
        REQUIRE(bool(int_t(-4) <= 4u));

        // >
        REQUIRE(bool(int_t(5) > int_t(4)));
        REQUIRE(!(int_t(4) > int_t(5)));
        REQUIRE(!(int_t(5) > int_t(5)));
        REQUIRE(bool(uint_t(5u) > int_t(4)));
        REQUIRE(bool(uint_t(5u) > int_t(-4)));
        REQUIRE(!(uint_t(4u) > int_t(5)));
        REQUIRE(!(uint_t(5u) > int_t(5)));
        REQUIRE(bool(int_t(5) > uint_t(4u)));
        REQUIRE(!(int_t(-5) > uint_t(4u)));
        REQUIRE(!(int_t(4) > uint_t(5u)));
        REQUIRE(!(int_t(5) > uint_t(5u)));

        REQUIRE(bool(5 > int_t(4)));
        REQUIRE(!(4 > int_t(5)));
        REQUIRE(!(5 > int_t(5)));
        REQUIRE(bool(5u > int_t(4)));
        REQUIRE(!(4u > int_t(5)));
        REQUIRE(!(5u > int_t(5)));
        REQUIRE(bool(5u > int_t(4)));
        REQUIRE(bool(5u > int_t(-4)));
        REQUIRE(!(4u > int_t(5)));
        REQUIRE(!(5u > int_t(5)));
        REQUIRE(bool(5 > uint_t(4u)));
        REQUIRE(!(-5 > uint_t(4u)));
        REQUIRE(!(4 > uint_t(5u)));
        REQUIRE(!(5 > uint_t(5u)));

        REQUIRE(bool(int_t(5) > 4));
        REQUIRE(!(int_t(4) > 5));
        REQUIRE(!(int_t(5) > 5));
        REQUIRE(bool(uint_t(5u) > 4));
        REQUIRE(bool(uint_t(5u) > -4));
        REQUIRE(!(uint_t(4u) > 5));
        REQUIRE(!(uint_t(5u) > 5));
        REQUIRE(bool(int_t(5) > 4u));
        REQUIRE(!(int_t(-5) > 4u));
        REQUIRE(!(int_t(4) > 5u));
        REQUIRE(!(int_t(5) > 5u));

        // >=
        REQUIRE(bool(int_t(5) >= int_t(4)));
        REQUIRE(!(int_t(4) >= int_t(5)));
        REQUIRE(bool(int_t(5) >= int_t(5)));
        REQUIRE(bool(uint_t(5u) >= int_t(4)));
        REQUIRE(!(uint_t(4u) >= int_t(5)));
        REQUIRE(bool(uint_t(4u) >= int_t(-5)));
        REQUIRE(bool(uint_t(5u) >= int_t(5)));
        REQUIRE(bool(int_t(5) >= uint_t(4u)));
        REQUIRE(!(int_t(4) >= uint_t(5u)));
        REQUIRE(bool(int_t(5) >= uint_t(5u)));
        REQUIRE(!(int_t(-5) >= uint_t(5u)));

        REQUIRE(bool(5 >= int_t(4)));
        REQUIRE(!(4 >= int_t(5)));
        REQUIRE(bool(5 >= int_t(5)));
        REQUIRE(bool(5u >= int_t(4)));
        REQUIRE(!(4u >= int_t(5)));
        REQUIRE(bool(4u >= int_t(-5)));
        REQUIRE(bool(5u >= int_t(5)));
        REQUIRE(bool(5u >= int_t(-5)));
        REQUIRE(bool(5 >= uint_t(4u)));
        REQUIRE(!(4 >= uint_t(5u)));
        REQUIRE(bool(5 >= uint_t(5u)));
        REQUIRE(!(-5 >= uint_t(5u)));

        REQUIRE(bool(int_t(5) >= 4));
        REQUIRE(!(int_t(4) >= 5));
        REQUIRE(bool(int_t(5) >= 5));
        REQUIRE(bool(uint_t(5u) >= 4));
        REQUIRE(!(uint_t(4u) >= 5));
        REQUIRE(bool(uint_t(4u) >= -5));
        REQUIRE(bool(uint_t(5u) >= 5));
        REQUIRE(bool(int_t(5) >= 4u));
        REQUIRE(!(int_t(-5) >= 4u));
        REQUIRE(!(int_t(4) >= 5u));
        REQUIRE(bool(int_t(5) >= 5u));
    }
    SUBCASE("make_(un)signed")
    {
        int_t             a = 5;
        integer<unsigned> b = make_unsigned(a);
        REQUIRE(static_cast<unsigned>(b) == 5);

        b = 125u;
        a = make_signed(b);
        REQUIRE(static_cast<int>(a) == 125);
    }
    SUBCASE("i/o")
    {
        std::ostringstream out;
        std::istringstream in("10");

        int_t i(0);
        out << i;
        REQUIRE(out.str() == "0");

        in >> i;
        REQUIRE(static_cast<int>(i) == 10);
    }
    SUBCASE("abs")
    {
        int               i  = 123;
        integer<unsigned> ia = collie::ts::abs(i);
        REQUIRE(static_cast<unsigned>(ia) == 123u);

        i  = -123;
        ia = collie::ts::abs(i);
        REQUIRE(static_cast<unsigned>(ia) == 123u);
    }
    SUBCASE("signed_to_unsigned")
    {
        // From int8_t
        REQUIRE(!std::is_convertible<integer<int8_t>, integer<uint8_t>>::value);
        REQUIRE(!std::is_convertible<integer<int8_t>, integer<uint16_t>>::value);
        REQUIRE(!std::is_convertible<integer<int8_t>, integer<uint32_t>>::value);
        REQUIRE(!std::is_convertible<integer<int8_t>, integer<uint64_t>>::value);

        // From int16_t
        REQUIRE(!std::is_convertible<integer<int16_t>, integer<uint8_t>>::value);
        REQUIRE(!std::is_convertible<integer<int16_t>, integer<uint16_t>>::value);
        REQUIRE(!std::is_convertible<integer<int16_t>, integer<uint32_t>>::value);
        REQUIRE(!std::is_convertible<integer<int16_t>, integer<uint64_t>>::value);

        // From int32_t
        REQUIRE(!std::is_convertible<integer<int32_t>, integer<uint8_t>>::value);
        REQUIRE(!std::is_convertible<integer<int32_t>, integer<uint16_t>>::value);
        REQUIRE(!std::is_convertible<integer<int32_t>, integer<uint32_t>>::value);
        REQUIRE(!std::is_convertible<integer<int32_t>, integer<uint64_t>>::value);

        // From int64_t
        REQUIRE(!std::is_convertible<integer<int64_t>, integer<uint8_t>>::value);
        REQUIRE(!std::is_convertible<integer<int64_t>, integer<uint16_t>>::value);
        REQUIRE(!std::is_convertible<integer<int64_t>, integer<uint32_t>>::value);
        REQUIRE(!std::is_convertible<integer<int64_t>, integer<uint64_t>>::value);
    }
    SUBCASE("unsigned_to_signed")
    {
        // From uint8_t
        REQUIRE(!std::is_convertible<integer<uint8_t>, integer<int8_t>>::value);
        REQUIRE(std::is_convertible<integer<uint8_t>, integer<int16_t>>::value);
        REQUIRE(std::is_convertible<integer<uint8_t>, integer<int32_t>>::value);
        REQUIRE(std::is_convertible<integer<uint8_t>, integer<int64_t>>::value);

        // From uint16_t
        REQUIRE(!std::is_convertible<integer<uint16_t>, integer<int8_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint16_t>, integer<int16_t>>::value);
        REQUIRE(std::is_convertible<integer<uint16_t>, integer<int32_t>>::value);
        REQUIRE(std::is_convertible<integer<uint16_t>, integer<int64_t>>::value);

        // From uint32_t
        REQUIRE(!std::is_convertible<integer<uint32_t>, integer<int8_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint32_t>, integer<int16_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint32_t>, integer<int32_t>>::value);
        REQUIRE(std::is_convertible<integer<uint32_t>, integer<int64_t>>::value);

        // From uint32_t
        REQUIRE(!std::is_convertible<integer<uint64_t>, integer<int8_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint64_t>, integer<int16_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint64_t>, integer<int32_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint64_t>, integer<int64_t>>::value);
    }
    SUBCASE("signed_to_signed")
    {
        // From int8_t
        REQUIRE(std::is_convertible<integer<int8_t>, integer<int8_t>>::value);
        REQUIRE(std::is_convertible<integer<int8_t>, integer<int16_t>>::value);
        REQUIRE(std::is_convertible<integer<int8_t>, integer<int32_t>>::value);
        REQUIRE(std::is_convertible<integer<int8_t>, integer<int64_t>>::value);

        // From int16_t
        REQUIRE(!std::is_convertible<integer<int16_t>, integer<int8_t>>::value);
        REQUIRE(std::is_convertible<integer<int16_t>, integer<int16_t>>::value);
        REQUIRE(std::is_convertible<integer<int16_t>, integer<int32_t>>::value);
        REQUIRE(std::is_convertible<integer<int16_t>, integer<int64_t>>::value);

        // From int32_t
        REQUIRE(!std::is_convertible<integer<int32_t>, integer<int8_t>>::value);
        REQUIRE(!std::is_convertible<integer<int32_t>, integer<int16_t>>::value);
        REQUIRE(std::is_convertible<integer<int32_t>, integer<int32_t>>::value);
        REQUIRE(std::is_convertible<integer<int32_t>, integer<int64_t>>::value);

        // From int64_t
        REQUIRE(!std::is_convertible<integer<int64_t>, integer<int8_t>>::value);
        REQUIRE(!std::is_convertible<integer<int64_t>, integer<int16_t>>::value);
        REQUIRE(!std::is_convertible<integer<int64_t>, integer<int32_t>>::value);
        REQUIRE(std::is_convertible<integer<int64_t>, integer<int64_t>>::value);
    }
    SUBCASE("unsigned_to_unsigned")
    {
        // From uint8_t
        REQUIRE(std::is_convertible<integer<uint8_t>, integer<uint8_t>>::value);
        REQUIRE(std::is_convertible<integer<uint8_t>, integer<uint16_t>>::value);
        REQUIRE(std::is_convertible<integer<uint8_t>, integer<uint32_t>>::value);
        REQUIRE(std::is_convertible<integer<uint8_t>, integer<uint64_t>>::value);

        // From uint16_t
        REQUIRE(!std::is_convertible<integer<uint16_t>, integer<uint8_t>>::value);
        REQUIRE(std::is_convertible<integer<uint16_t>, integer<uint16_t>>::value);
        REQUIRE(std::is_convertible<integer<uint16_t>, integer<uint32_t>>::value);
        REQUIRE(std::is_convertible<integer<uint16_t>, integer<uint64_t>>::value);

        // From uint32_t
        REQUIRE(!std::is_convertible<integer<uint32_t>, integer<uint8_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint32_t>, integer<uint16_t>>::value);
        REQUIRE(std::is_convertible<integer<uint32_t>, integer<uint32_t>>::value);
        REQUIRE(std::is_convertible<integer<uint32_t>, integer<uint64_t>>::value);

        // From uint64_t
        REQUIRE(!std::is_convertible<integer<uint64_t>, integer<uint8_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint64_t>, integer<uint16_t>>::value);
        REQUIRE(!std::is_convertible<integer<uint64_t>, integer<uint32_t>>::value);
        REQUIRE(std::is_convertible<integer<uint64_t>, integer<uint64_t>>::value);
    }
}
