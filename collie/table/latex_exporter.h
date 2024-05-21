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

#include <collie/table/exporter.h>
#include <optional>

namespace collie::table {

    class LatexExporter : public Exporter {

        static const char new_line = '\n';

    public:
        class ExportOptions {
        public:
            ExportOptions &indentation(std::size_t value) {
                indentation_ = value;
                return *this;
            }

        private:
            friend class LatexExporter;

            std::optional<size_t> indentation_;
        };

        ExportOptions &configure() { return options_; }

        std::string dump(Table &table) override {
            std::string result{"\\begin{tabular}"};
            result += new_line;

            result += add_alignment_header(table);
            result += new_line;
            const auto rows = table.rows_;
            // iterate content and put text into the table.
            for (size_t i = 0; i < rows; i++) {
                auto &row = table[i];
                // apply row content indentation
                if (options_.indentation_.has_value()) {
                    result += std::string(options_.indentation_.value(), ' ');
                }

                for (size_t j = 0; j < row.size(); j++) {

                    result += row[j].get_text();

                    // check column position, need "\\" at the end of each row
                    if (j < row.size() - 1) {
                        result += " & ";
                    } else {
                        result += " \\\\";
                    }
                }
                result += new_line;
            }

            result += "\\end{tabular}";
            return result;
        }

        virtual ~LatexExporter() {}

    private:
        std::string add_alignment_header(Table &table) {
            std::string result{"{"};

            for (auto &cell: table[0]) {
                auto format = cell.format();
                if (format.font_align_.value() == FontAlign::left) {
                    result += 'l';
                } else if (format.font_align_.value() == FontAlign::center) {
                    result += 'c';
                } else if (format.font_align_.value() == FontAlign::right) {
                    result += 'r';
                }
            }

            result += "}";
            return result;
        }

        ExportOptions options_;
    };

} // namespace collie::table
