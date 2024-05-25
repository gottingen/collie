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


/*
 * Make sure the inclusion works correctly without _GNU_SOURCE
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <collie/simd/simd.h>

#include <collie/testing/doctest.h>

TEST_CASE("[GNU_SOURCE support]")
{

    SUBCASE("exp10")
    {
        CHECK_EQ(collie::simd::exp10(0.), 1.);
        CHECK_EQ(collie::simd::exp10(0.f), 1.f);
    }
}
