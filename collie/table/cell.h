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

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <collie/table/format.h>
#include <collie/table/utf8.h>
#include <optional>
#include <vector>

namespace collie::table {

    class Cell {
    public:
        explicit Cell(std::shared_ptr<class Row> parent) : parent_(parent) {}

        void set_text(const std::string &text) { data_ = text; }

        const std::string &get_text() { return data_; }

        size_t size() {
            return get_sequence_length(data_, locale(), is_multi_byte_character_support_enabled());
        }

        std::string locale() { return *format().locale_; }

        Format &format();

        bool is_multi_byte_character_support_enabled();

    private:
        std::string data_;
        std::weak_ptr<class Row> parent_;
        std::optional<Format> format_;
    };

} // namespace collie::table
