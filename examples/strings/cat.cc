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

#include <collie/strings/cat.h>
#include <iostream>

void str_cat_example() {
    std::cout << "number: "<<collie::str_cat(1, 2, 3, 4, 5) << std::endl;
    std::cout << "string: "<<collie::str_cat("a", "b", "c", "d", "e") << std::endl;
    std::cout << "vector string: "<<collie::str_cat(std::vector<std::string>{"a", "b", "c", "d", "e"}) << std::endl;
    std::cout << "vector string_view: "<<collie::str_cat(std::vector<std::string_view>{"a", "b", "c", "d", "e"}) << std::endl;
}

void str_cat_append_example() {
    std::string result("number: ");
    collie::str_cat_append(result, 1);
    collie::str_cat_append(result, 2);
    collie::str_cat_append(result, 3);
    collie::str_cat_append(result, 4);
    collie::str_cat_append(result, 5);
    std::cout << result << std::endl;
}
int main() {
    str_cat_example();
    str_cat_append_example();
    return 0;
}