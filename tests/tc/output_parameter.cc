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
#include <collie/type_safe/output_parameter.h>

#include <collie/testing/doctest.h>
#include <string>

using namespace collie::ts;

TEST_CASE("output_parameter")
{
    SUBCASE("reference") {
        std::string output;

        output_parameter<std::string> out(output);
        SUBCASE("operator=") {
            std::string &res = (out = "abc");
            REQUIRE(&res == &output);
            REQUIRE(res == "abc");
        }
        SUBCASE("assign") {
            std::string &res = out.assign(3u, 'c');
            REQUIRE(&res == &output);
            REQUIRE(res == "ccc");
        }
        SUBCASE("multi") {
            std::string &res_a = (out = "abc");
            REQUIRE(&res_a == &output);
            REQUIRE(res_a == "abc");

            std::string &res_b = (out = "def");
            REQUIRE(&res_b == &output);
            REQUIRE(res_b == "def");

            std::string &res_c = out.assign(3u, 'c');
            REQUIRE(&res_c == &output);
            REQUIRE(res_c == "ccc");
        }
    }
    SUBCASE("deferred_construction") {
        deferred_construction<std::string> output;

        output_parameter<std::string> out(output);
        SUBCASE("operator=") {
            std::string &res = (out = "abc");
            REQUIRE(output.has_value());
            REQUIRE(&res == &output.value());
            REQUIRE(res == "abc");
        }
        SUBCASE("assign") {
            std::string &res = out.assign(3u, 'c');
            REQUIRE(output.has_value());
            REQUIRE(&res == &output.value());
            REQUIRE(res == "ccc");
        }
        SUBCASE("multi") {
            std::string &res_a = (out = "abc");
            REQUIRE(output.has_value());
            REQUIRE(&res_a == &output.value());
            REQUIRE(res_a == "abc");

            std::string &res_b = (out = "def");
            REQUIRE(&res_b == &output.value());
            REQUIRE(res_b == "def");

            std::string &res_c = out.assign(3u, 'c');
            REQUIRE(&res_c == &output.value());
            REQUIRE(res_c == "ccc");
        }
    }
}
