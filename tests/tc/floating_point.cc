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
#include <collie/type_safe/floating_point.h>

#include <collie/testing/doctest.h>

#include <sstream>

using namespace collie::ts;

#ifndef TYPE_SAFE_TEST_NO_STATIC_ASSERT
static_assert(std::is_trivially_copyable<floating_point<float>>::value, "");
static_assert(std::is_standard_layout<floating_point<float>>::value, "");
// conversion checks
static_assert(std::is_constructible<floating_point<float>, float>::value, "");
static_assert(!std::is_constructible<floating_point<float>, double>::value, "");
static_assert(std::is_constructible<floating_point<double>, double>::value, "");
static_assert(std::is_constructible<floating_point<double>, double>::value, "");
static_assert(!std::is_constructible<floating_point<double>, long double>::value, "");
static_assert(std::is_assignable<floating_point<float>, float>::value, "");
static_assert(!std::is_assignable<floating_point<float>, double>::value, "");
static_assert(std::is_assignable<floating_point<double>, double>::value, "");
static_assert(std::is_assignable<floating_point<double>, double>::value, "");
static_assert(!std::is_assignable<floating_point<double>, long double>::value, "");
#endif

TEST_CASE("floating_point")
{
    using float_t = floating_point<double>;

    SUBCASE("constructor")
    {
        float_t a(0.);
        REQUIRE(static_cast<double>(a) == 0.);
        float_t b(3.14);
        REQUIRE(static_cast<double>(b) == 3.14);
        float_t c(-42.5);
        REQUIRE(static_cast<double>(c) == -42.5);
    }
    SUBCASE("assignment")
    {
        float_t a(0.);
        a = 3.14;
        REQUIRE(static_cast<double>(a) == 3.14);
        a = -42.5;
        REQUIRE(static_cast<double>(a) == -42.5);
    }
    SUBCASE("unary")
    {
        float_t a(13.255);
        REQUIRE(static_cast<double>(+a) == static_cast<double>(a));
        REQUIRE(static_cast<double>(-a) == -static_cast<double>(a));
    }
    SUBCASE("addition")
    {
        float_t wrapper(0.);
        double  normal(0.);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper += 4.5;
        normal += 4.5;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = wrapper + (-2.3);
        normal  = normal + (-2.3);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = 1.1 + wrapper;
        normal  = 1.1 + normal;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = float_t(-11.0) + wrapper;
        normal  = -11.0 + normal;
        REQUIRE(static_cast<double>(wrapper) == normal);
    }
    SUBCASE("subtraction")
    {
        float_t wrapper(0.);
        double  normal(0.);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper -= 4.5;
        normal -= 4.5;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = wrapper - (-2.3);
        normal  = normal - (-2.3);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = 1.1 - wrapper;
        normal  = 1.1 - normal;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = float_t(-11.0) - wrapper;
        normal  = -11.0 - normal;
        REQUIRE(static_cast<double>(wrapper) == normal);
    }
    SUBCASE("multiplication")
    {
        float_t wrapper(1.);
        double  normal(1.);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper *= 4.5;
        normal *= 4.5;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = wrapper * (-2.3);
        normal  = normal * (-2.3);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = 1.1 * wrapper;
        normal  = 1.1 * normal;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = float_t(-11.0) * wrapper;
        normal  = -11.0 * normal;
        REQUIRE(static_cast<double>(wrapper) == normal);
    }
    SUBCASE("division")
    {
        float_t wrapper(10.);
        double  normal(10.);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper /= 4.5;
        normal /= 4.5;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = wrapper / (-2.3);
        normal  = normal / (-2.3);
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = 1.1 / wrapper;
        normal  = 1.1 / normal;
        REQUIRE(static_cast<double>(wrapper) == normal);

        wrapper = float_t(-11.0) / wrapper;
        normal  = -11.0 / normal;
        REQUIRE(static_cast<double>(wrapper) == normal);
    }
    SUBCASE("comparison")
    {
        // <
        REQUIRE(bool(float_t(4.) < float_t(5.)));
        REQUIRE(!(float_t(5.) < float_t(4.)));
        REQUIRE(!(float_t(4.) < float_t(4.)));

        REQUIRE(bool(4. < float_t(5.)));
        REQUIRE(!(5. < float_t(4.)));
        REQUIRE(!(4. < float_t(4.)));

        REQUIRE(bool(float_t(4.) < 5.));
        REQUIRE(!(float_t(5.) < 4.));
        REQUIRE(!(float_t(4.) < 4.));

        // <=
        REQUIRE(bool(float_t(4.) <= float_t(5.)));
        REQUIRE(!(float_t(5.) <= float_t(4.)));
        REQUIRE(bool(float_t(4.) <= float_t(4.)));

        REQUIRE(bool(4. <= float_t(5.)));
        REQUIRE(!(5. <= float_t(4.)));
        REQUIRE(bool(4. <= float_t(4.)));

        REQUIRE(bool(float_t(4.) <= 5.));
        REQUIRE(!(float_t(5.) <= 4.));
        REQUIRE(bool(float_t(4.) <= 4.));

        // >
        REQUIRE(bool(float_t(5.) > float_t(4.)));
        REQUIRE(!(float_t(4.) > float_t(5.)));
        REQUIRE(!(float_t(5.) > float_t(5.)));

        REQUIRE(bool(5. > float_t(4.)));
        REQUIRE(!(4. > float_t(5.)));
        REQUIRE(!(5. > float_t(5.)));

        REQUIRE(bool(float_t(5.) > 4.));
        REQUIRE(!(float_t(4.) > 5.));
        REQUIRE(!(float_t(5.) > 5.));

        // >=
        REQUIRE(bool(float_t(5.) >= float_t(4.)));
        REQUIRE(!(float_t(4.) >= float_t(5.)));
        REQUIRE(bool(float_t(5.) >= float_t(5.)));

        REQUIRE(bool(5. >= float_t(4.)));
        REQUIRE(!(4. >= float_t(5.)));
        REQUIRE(bool(5. >= float_t(5.)));

        REQUIRE(bool(float_t(5.) >= 4.));
        REQUIRE(!(float_t(4.) >= 5.));
        REQUIRE(bool(float_t(5.) >= 5.));
    }
    SUBCASE("i/o")
    {
        std::ostringstream out;
        std::istringstream in("1.0");

        float_t f(0.0);
        out << f;
        REQUIRE(out.str() == "0");

        in >> f;
        REQUIRE(static_cast<double>(f) == 1.0);
    }
}
