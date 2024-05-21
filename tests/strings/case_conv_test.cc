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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <collie/testing/doctest.h>
#include <collie/strings/case_conv.h>

TEST_CASE("AsciiStrTo, Lower") {
    const char buf[] = "ABCDEF";
    const std::string str("GHIJKL");
    const std::string str2("MNOPQR");
    const std::string_view sp(str2);
    std::string mutable_str("STUVWX");

    CHECK_EQ("abcdef", collie::str_to_lower(buf));
    CHECK_EQ("ghijkl", collie::str_to_lower(str));
    CHECK_EQ("mnopqr", collie::str_to_lower(sp));

    collie::str_to_lower(&mutable_str);
    CHECK_EQ("stuvwx", mutable_str);

    char mutable_buf[] = "Mutable";
    std::transform(mutable_buf, mutable_buf + strlen(mutable_buf),
                   mutable_buf, collie::ascii_to_lower);
    CHECK_EQ("mutable", std::string_view(mutable_buf));
}

TEST_CASE("AsciiStrTo, Upper") {
    const char buf[] = "abcdef";
    const std::string str("ghijkl");
    const std::string str2("mnopqr");
    const std::string_view sp(str2);

    CHECK_EQ("ABCDEF", collie::str_to_upper(buf));
    CHECK_EQ("GHIJKL", collie::str_to_upper(str));
    CHECK_EQ("MNOPQR", collie::str_to_upper(sp));

    char mutable_buf[] = "Mutable";
    std::transform(mutable_buf, mutable_buf + strlen(mutable_buf),
                   mutable_buf, collie::ascii_to_upper);
    CHECK_EQ("MUTABLE", std::string_view(mutable_buf));
}

