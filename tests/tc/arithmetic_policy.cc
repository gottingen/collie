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

#include <collie/type_safe/arithmetic_policy.h>

#include <collie/testing/doctest.h>

using namespace collie::ts;

TEST_CASE("over/underflow")
{
    SUBCASE("unsigned")
    {
        auto max = std::numeric_limits<unsigned>::max();

        REQUIRE(detail::will_addition_error(detail::unsigned_integer_tag{}, max, 1u));
        REQUIRE(detail::will_addition_error(detail::unsigned_integer_tag{}, max - 5u, 10u));
        REQUIRE(detail::will_addition_error(detail::unsigned_integer_tag{}, max - 1u, max));
        REQUIRE(!detail::will_addition_error(detail::unsigned_integer_tag{}, 0u, max));
        REQUIRE(!detail::will_addition_error(detail::unsigned_integer_tag{}, 1u, max - 1u));

        REQUIRE(detail::will_subtraction_error(detail::unsigned_integer_tag{}, 0u, 1u));
        REQUIRE(detail::will_subtraction_error(detail::unsigned_integer_tag{}, 5u, 6u));
        REQUIRE(detail::will_subtraction_error(detail::unsigned_integer_tag{}, 0u, max));
        REQUIRE(!detail::will_subtraction_error(detail::unsigned_integer_tag{}, 5u, 5u));

        REQUIRE(detail::will_multiplication_error(detail::unsigned_integer_tag{}, max, max));
        REQUIRE(detail::will_multiplication_error(detail::unsigned_integer_tag{}, max, 2u));
        REQUIRE(detail::will_multiplication_error(detail::unsigned_integer_tag{}, max / 2u, 3u));
        REQUIRE(!detail::will_multiplication_error(detail::unsigned_integer_tag{}, max / 3u, 3u));

        REQUIRE(detail::will_division_error(detail::unsigned_integer_tag{}, 1u, 0u));
        REQUIRE(!detail::will_division_error(detail::unsigned_integer_tag{}, 1u, 1u));

        REQUIRE(detail::will_modulo_error(detail::unsigned_integer_tag{}, 1u, 0u));
        REQUIRE(!detail::will_modulo_error(detail::unsigned_integer_tag{}, 1u, 1u));
    }
    SUBCASE("signed")
    {
        auto max = std::numeric_limits<int>::max();
        auto min = std::numeric_limits<int>::min();

        REQUIRE(detail::will_addition_error(detail::signed_integer_tag{}, max, 1));
        REQUIRE(detail::will_addition_error(detail::signed_integer_tag{}, max - 5, 10));
        REQUIRE(detail::will_addition_error(detail::signed_integer_tag{}, max - 1, max));
        REQUIRE(!detail::will_addition_error(detail::signed_integer_tag{}, 0, max));
        REQUIRE(!detail::will_addition_error(detail::signed_integer_tag{}, 1, max - 1));
        REQUIRE(detail::will_addition_error(detail::signed_integer_tag{}, min, -1));
        REQUIRE(detail::will_addition_error(detail::signed_integer_tag{}, min + 5, -10));
        REQUIRE(!detail::will_addition_error(detail::signed_integer_tag{}, 0, min));
        REQUIRE(!detail::will_addition_error(detail::signed_integer_tag{}, -1, min + 1));

        REQUIRE(detail::will_subtraction_error(detail::signed_integer_tag{}, min, 1));
        REQUIRE(detail::will_subtraction_error(detail::signed_integer_tag{}, min + 5, 6));
        REQUIRE(detail::will_subtraction_error(detail::signed_integer_tag{}, min, max));
        REQUIRE(!detail::will_subtraction_error(detail::signed_integer_tag{}, 5, 5));
        REQUIRE(detail::will_subtraction_error(detail::signed_integer_tag{}, max, -1));
        REQUIRE(detail::will_subtraction_error(detail::signed_integer_tag{}, max - 5, -6));
        REQUIRE(!detail::will_subtraction_error(detail::signed_integer_tag{}, 5, -5));

        REQUIRE(detail::will_multiplication_error(detail::signed_integer_tag{}, max, max));
        REQUIRE(detail::will_multiplication_error(detail::signed_integer_tag{}, max, 2));
        REQUIRE(detail::will_multiplication_error(detail::signed_integer_tag{}, max / 2, 3));
        REQUIRE(!detail::will_multiplication_error(detail::signed_integer_tag{}, max / 3, 3));
        REQUIRE(detail::will_multiplication_error(detail::signed_integer_tag{}, max, min));
        REQUIRE(detail::will_multiplication_error(detail::signed_integer_tag{}, max, -2));
        REQUIRE(detail::will_multiplication_error(detail::signed_integer_tag{}, max / 2, -3));
        REQUIRE(!detail::will_multiplication_error(detail::signed_integer_tag{}, max / 3, -3));

        REQUIRE(detail::will_division_error(detail::signed_integer_tag{}, 1, 0));
        REQUIRE(detail::will_division_error(detail::signed_integer_tag{}, min, -1));
        REQUIRE(!detail::will_division_error(detail::signed_integer_tag{}, 1, 1));

        REQUIRE(detail::will_modulo_error(detail::signed_integer_tag{}, 1, 0));
        REQUIRE(!detail::will_modulo_error(detail::signed_integer_tag{}, 1, 1));
    }
}
