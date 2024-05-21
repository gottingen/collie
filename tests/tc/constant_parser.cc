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
#include <collie/type_safe/detail/constant_parser.h>

#include <collie/testing/doctest.h>

using namespace collie::ts;

TEST_CASE("detail::parse")
{
    REQUIRE((detail::parse<int, '0'>() == 0));
    REQUIRE((detail::parse<int, '1', '0'>() == 10));
    REQUIRE((detail::parse<int, '4', '2', '3'>() == 423));
    REQUIRE((detail::parse<int, '2', '3', '\'', '9', '0', '0'>() == 23900));

    REQUIRE((detail::parse<int, '0', '1', '0'>() == 8));
    REQUIRE((detail::parse<int, '0', 'x', 'A'>() == 10));
    REQUIRE((detail::parse<int, '0', 'b', '1'>() == 1));
}
