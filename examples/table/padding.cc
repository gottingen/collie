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
using namespace collie;
using Row_t = Table::Row_t;

int main() {
  Table no_padding;
  no_padding.format().font_style({FontStyle::bold}).padding(0);
  no_padding.add_row(Row_t{"This cell has no padding"});
  std::cout << "Table with no padding:\n" << no_padding << std::endl;

  Table padding_top_bottom;
  padding_top_bottom.format().font_style({FontStyle::bold}).padding(0);
  padding_top_bottom.add_row(
      Row_t{"This cell has padding top = 1", "This cell has padding bottom = 1"});
  padding_top_bottom[0][0].format().padding_top(1);
  padding_top_bottom[0][1].format().padding_bottom(1);

  std::cout << "\nTable with top/bottom padding:\n" << padding_top_bottom << std::endl;
}