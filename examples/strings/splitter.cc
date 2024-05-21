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

#include <collie/strings/splitter.h>
#include <collie/strings/str_split.h>
#include <collie/strings/format.h>
#include <iostream>

extern std::vector<std::string> split_vector(const std::string &str, const std::string &delim);
int main() {
    std::string str = "a,b,,c,d,e ";
    collie::StringSplitter splitter(str, ',');
    for (; splitter; ++splitter) {
        auto s = collie::format("splitter: {}\n", splitter.field_sp());
        std::cout << s;
    }

    std::string str2 = "a,b,,c,d,e ";
    auto v = split_vector(str2, ",");
    std::vector<std::string> vl = collie::str_split(str2, ",");
    collie::println("split_vector: {}", v.size());
    for (const auto &s : v) {
       collie::println("split_vector: {}", s);
    }
    return 0;
}