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
int main() {
  Table styled_table;
  styled_table.add_row(Row_t{"Bold", "Italic", "Bold & Italic", "Blinking"});
  styled_table.add_row(Row_t{"Underline", "Crossed", "Dark", "Bold, Italic & Underlined"});

  styled_table[0][0].format().font_style({FontStyle::bold});

  styled_table[0][1].format().font_style({FontStyle::italic});

  styled_table[0][2].format().font_style({FontStyle::bold, FontStyle::italic});

  styled_table[0][3].format().font_style({FontStyle::blink});

  styled_table[1][0].format().font_style({FontStyle::underline});

  styled_table[1][1].format().font_style({FontStyle::crossed});

  styled_table[1][2].format().font_style({FontStyle::dark});

  styled_table[1][3].format().font_style(
      {FontStyle::bold, FontStyle::italic, FontStyle::underline});

  std::cout << styled_table << std::endl;
}