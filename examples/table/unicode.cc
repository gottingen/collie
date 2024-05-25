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
    Table table;

    table.format()
            .corner("♥")
            .font_style({FontStyle::bold})
            .corner_color(Color::magenta)
            .border_color(Color::magenta);

    table.add_row(Row_t{"English", "I love you"});
    table.add_row(Row_t{"French", "Je t’aime"});
    table.add_row(Row_t{"Spanish", "Te amo"});
    table.add_row(Row_t{"German", "Ich liebe Dich"});
    table.add_row(Row_t{"Mandarin Chinese", "我爱你"});
    table.add_row(Row_t{"Japanese", "愛してる"});
    table.add_row(Row_t{"Korean", "사랑해 (Saranghae)"});
    table.add_row(Row_t{"Greek", "Σ΄αγαπώ (Se agapo)"});
    table.add_row(Row_t{"Italian", "Ti amo"});
    table.add_row(Row_t{"Russian", "Я тебя люблю (Ya tebya liubliu)"});
    table.add_row(Row_t{"Hebrew", "אני אוהב אותך (Ani ohev otakh)"});

    // Column 1 is using mult-byte characters
    table.column(1).format().multi_byte_characters(true);

    std::cout << table << std::endl;
}
