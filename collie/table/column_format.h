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

namespace collie::table {

    class ColumnFormat : public Format {
    public:
        explicit ColumnFormat(class Column &column) : column_(column) {}

        ColumnFormat &width(size_t value);

        ColumnFormat &height(size_t value);

        // Padding
        ColumnFormat &padding(size_t value);

        ColumnFormat &padding_left(size_t value);

        ColumnFormat &padding_right(size_t value);

        ColumnFormat &padding_top(size_t value);

        ColumnFormat &padding_bottom(size_t value);

        // Border
        ColumnFormat &border(const std::string &value);

        ColumnFormat &border_color(Color value);

        ColumnFormat &border_background_color(Color value);

        ColumnFormat &border_left(const std::string &value);

        ColumnFormat &border_left_color(Color value);

        ColumnFormat &border_left_background_color(Color value);

        ColumnFormat &border_right(const std::string &value);

        ColumnFormat &border_right_color(Color value);

        ColumnFormat &border_right_background_color(Color value);

        ColumnFormat &border_top(const std::string &value);

        ColumnFormat &border_top_color(Color value);

        ColumnFormat &border_top_background_color(Color value);

        ColumnFormat &border_bottom(const std::string &value);

        ColumnFormat &border_bottom_color(Color value);

        ColumnFormat &border_bottom_background_color(Color value);

        // Corner
        ColumnFormat &corner(const std::string &value);

        ColumnFormat &corner_color(Color value);

        ColumnFormat &corner_background_color(Color value);

        // Column separator
        ColumnFormat &column_separator(const std::string &value);

        ColumnFormat &column_separator_color(Color value);

        ColumnFormat &column_separator_background_color(Color value);

        // Font styling
        ColumnFormat &font_align(FontAlign value);

        ColumnFormat &font_style(const std::vector<FontStyle> &style);

        ColumnFormat &font_color(Color value);

        ColumnFormat &font_background_color(Color value);

        ColumnFormat &color(Color value);

        ColumnFormat &background_color(Color value);

        // Locale
        ColumnFormat &multi_byte_characters(bool value);

        ColumnFormat &locale(const std::string &value);

    private:
        std::reference_wrapper<class Column> column_;
    };

} // namespace collie::table
