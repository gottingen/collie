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

#include <collie/table/table.h>
using namespace collie::table;
using Row_t = Table::Row_t;
using namespace collie;
void print_shape(Table &table) {
  auto shape = table.shape();
  std::cout << "Shape: (" << shape.first << ", " << shape.second << ")" << std::endl;
}

int main() {
  Table table;
  table.add_row(Row_t{"Command", "Description"});
  table.add_row(Row_t{"git status", "List all new or modified files"});
  table.add_row(Row_t{"git diff", "Show file differences that haven't been staged"});
  std::cout << table << std::endl;
  print_shape(table);
}
