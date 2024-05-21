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
#ifndef TYPE_SAFE_TEST_DEBUGGER_TYPE_HPP_INCLUDED
#define TYPE_SAFE_TEST_DEBUGGER_TYPE_HPP_INCLUDED

struct debugger_type
{
    int  id;
    bool from_ctor         = false;
    bool from_move_ctor    = false;
    bool from_copy_ctor    = false;
    bool was_move_assigned = false;
    bool was_copy_assigned = false;
    bool swapped           = false;

    debugger_type(int id, double = 0., char = 0) : id(id)
    {
        from_ctor = true;
    }

    debugger_type(debugger_type&& other) : debugger_type(other.id)
    {
        from_move_ctor = true;
    }

    debugger_type(const debugger_type& other) : debugger_type(other.id)
    {
        from_copy_ctor = true;
    }

    ~debugger_type() = default;

    debugger_type& operator=(debugger_type&& other)
    {
        id                = other.id;
        was_move_assigned = true;
        return *this;
    }

    debugger_type& operator=(const debugger_type& other)
    {
        id                = other.id;
        was_copy_assigned = true;
        return *this;
    }

    friend void swap(debugger_type& a, debugger_type& b) noexcept
    {
        std::swap(a.id, b.id);
        a.swapped = b.swapped = true;
    }

    bool ctor() const
    {
        return from_ctor;
    }

    bool move_ctor() const
    {
        return !from_copy_ctor;
    }

    bool copy_ctor() const
    {
        return !from_move_ctor;
    }

    bool not_assigned() const
    {
        return !was_copy_assigned && !was_move_assigned;
    }

    bool move_assigned() const
    {
        return was_move_assigned && !was_copy_assigned;
    }

    bool copy_assigned() const
    {
        return was_copy_assigned && !was_move_assigned;
    }
};

inline bool operator==(const debugger_type& a, const debugger_type& b)
{
    return a.id == b.id;
}

inline bool operator<(const debugger_type& a, const debugger_type& b)
{
    return a.id < b.id;
}

#endif // TYPE_SAFE_TEST_DEBUGGER_TYPE_HPP_INCLUDED
