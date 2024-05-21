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

#include <iomanip>
#include <collie/table/table.h>
using namespace collie::table;
using Row_t = Table::Row_t;

int main() {
  Table employees;

  Table department;
  department.add_row(Row_t{"Research", "Engineering"});

  RowStream rs;
  rs << std::setprecision(10);

  // Add rows
  // clang-format off
  employees.add_row(Row_t{"Emp. ID", "First Name", "Last Name", "Department / Business Unit", "Pay Rate"});
  employees.add_row(RowStream{}.copyfmt(rs) << 101 << "Donald" << "Patrick" << "Finance" << 59.61538461);
  employees.add_row(RowStream{}.copyfmt(rs) << 102 << "Rachel" << "Williams" << "Marketing and Operational\nLogistics Planning" << 34.97067307);
  employees.add_row(RowStream{}.copyfmt(rs) << 103 << "Ian" << "Jacob" << department << 57.00480769);
  // clang-format on

  employees.column(0)
      .format()
      .font_style({FontStyle::bold})
      .font_color(Color::white)
      .font_align(FontAlign::right);

  employees.column(3).format().font_align(FontAlign::center);

  // Print the table
  std::cout << employees << std::endl;
}