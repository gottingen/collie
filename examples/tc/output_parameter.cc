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

#include <iostream>
#include <sstream>
#include <string>

#include <collie/type_safe/output_parameter.h>

namespace ts = collie::ts;

// task: read strings from stream until EOF
// concatenate all strings
// return whether any strings read

// using a reference as output parameter - BAD:
// * not obvious from the caller that the string will be modified  (in the general case)
// * requires default constructor
// * implicit precondition that the output is empty
bool read_str_naive(std::istream& in, std::string& out)
{
    for (std::string tmp; in >> tmp;)
        out += tmp;
    return !out.empty();
}

// does not have these problems
bool read_str_better(std::istream& in, ts::output_parameter<std::string> out)
{
    std::string result; // no way to access the string directly
    // so need to create new one

    for (std::string tmp; in >> tmp;)
        result += tmp;

    // granted, that's verbose
    auto empty = result.empty();    // we need to query here, because after move it might be empty
    out        = std::move(result); // set output parameter
    return !empty;

    // so use this one:
    // assignment op returns the value that was assigned
    return (out = std::move(result)).empty();
}

int main()
{
    {
        std::istringstream in("hello world");
        std::string        str;
        auto               res = read_str_naive(in, str);
        std::cout << res << ' ' << str << '\n';
    }
    {
        std::istringstream in("hello world");
        std::string        str;
        // use ts::out() to create an output_parameter easily
        auto res = read_str_better(in, ts::out(str));
        std::cout << res << ' ' << str << '\n';
    }
    // what if std::string had no default constructor?
    {
        // use this one:
        ts::deferred_construction<std::string> str;
        // str is not initialized yet,
        // so it does not require a constructor
        // once you give it a value, it will never be empty again

        std::istringstream in("hello world");
        auto               res = read_str_better(in, ts::out(str));
        // if the function assigned a value to the output parameter,
        // it will call the constructor and directly initializes it with the correct value
        std::cout << res << ' ' << str.has_value() << ' ' << str.value() << '\n';
    }
}
