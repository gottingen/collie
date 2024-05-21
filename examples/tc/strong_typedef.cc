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
#include <string>
#include <unordered_set>

#include <collie/type_safe/strong_typedef.h>

namespace ts = collie::ts;

// we want some kind of handle to an int
struct handle : ts::strong_typedef<handle, int*>,                   // required
                ts::strong_typedef_op::equality_comparison<handle>, // for operator==/operator!=
                ts::strong_typedef_op::dereference<handle, int>     // for operator*/operator->
{
    using strong_typedef::strong_typedef;

    // we also want the operator bool()
    explicit operator bool() const noexcept
    {
        return static_cast<int*>(*this) != nullptr;
    }
};

void use_handle(handle h)
{
    // we can dereference it
    std::cout << *h << '\n';

    // and compare it
    (void)(h == handle(nullptr));

    // and reassign
    int a;
    h = handle(&a);
    // or get back
    int* ptr = static_cast<int*>(h);
    std::cout << &a << ' ' << ptr << '\n';
}

// integer representing a distance
struct distance
: ts::strong_typedef<distance, unsigned>,                 // required
  ts::strong_typedef_op::equality_comparison<distance>,   // for operator==/operator!=
  ts::strong_typedef_op::relational_comparison<distance>, // for operator< etc.
  ts::strong_typedef_op::integer_arithmetic<distance> // all arithmetic operators that make sense
                                                      // for integers
{
    using strong_typedef::strong_typedef;
};

// we want to output it
std::ostream& operator<<(std::ostream& out, const distance& d)
{
    return out << static_cast<unsigned>(d) << " distance";
}

namespace std
{
// we want to use it with the std::unordered_* containers
template <>
struct hash<::distance> : collie::ts::hashable<::distance>
{};

} // namespace std

int main()
{
    distance d(4);
    //    int      val = d; // error
    //    d += 3;           // error
    d += distance(3); // works

    std::unordered_set<distance> set{d};

    std::cout << *set.find(d) << '\n';
}
