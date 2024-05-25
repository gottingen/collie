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

#include <algorithm>
#include <optional>
#include <sstream>
#include <string>
#include <collie/table/exporter.h>

namespace collie::table {

    class AsciiDocExporter : public Exporter {

        static const char new_line = '\n';

    public:
        std::string dump(Table &table) override {
            std::stringstream ss;
            ss << add_alignment_header(table);
            ss << new_line;

            const auto rows = table.rows_;
            // iterate content and put text into the table.
            for (size_t row_index = 0; row_index < rows; row_index++) {
                auto &row = table[row_index];

                for (size_t cell_index = 0; cell_index < row.size(); cell_index++) {
                    ss << "|";
                    ss << add_formatted_cell(row[cell_index]);
                }
                ss << new_line;
                if (row_index == 0) {
                    ss << new_line;
                }
            }

            ss << "|===";
            return ss.str();
        }

        virtual ~AsciiDocExporter() {}

    private:
        std::string add_formatted_cell(Cell &cell) const {
            std::stringstream ss;
            auto format = cell.format();
            std::string cell_string = cell.get_text();

            auto font_style = format.font_style_.value();

            bool format_bold = false;
            bool format_italic = false;
            std::for_each(font_style.begin(), font_style.end(), [&](FontStyle &style) {
                if (style == FontStyle::bold) {
                    format_bold = true;
                } else if (style == FontStyle::italic) {
                    format_italic = true;
                }
            });

            if (format_bold) {
                ss << '*';
            }
            if (format_italic) {
                ss << '_';
            }

            ss << cell_string;
            if (format_italic) {
                ss << '_';
            }
            if (format_bold) {
                ss << '*';
            }
            return ss.str();
        }

        std::string add_alignment_header(Table &table) {
            std::stringstream ss;
            ss << (R"([cols=")");

            size_t column_count = table[0].size();
            size_t column_index = 0;
            for (auto &cell: table[0]) {
                auto format = cell.format();

                if (format.font_align_.value() == FontAlign::left) {
                    ss << '<';
                } else if (format.font_align_.value() == FontAlign::center) {
                    ss << '^';
                } else if (format.font_align_.value() == FontAlign::right) {
                    ss << '>';
                }

                ++column_index;
                if (column_index != column_count) {
                    ss << ",";
                }
            }

            ss << R"("])";
            ss << new_line;
            ss << "|===";

            return ss.str();
        }
    };

} // namespace collie::table
