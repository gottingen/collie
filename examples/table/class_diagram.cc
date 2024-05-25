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
using namespace collie;
using namespace collie::table;
using Row_t = Table::Row_t;

int main() {
  Table class_diagram;

  // Global styling
  class_diagram.format().font_style({FontStyle::bold}).font_align(FontAlign::center).width(60);

  // Animal class
  Table animal;
  animal.add_row(Row_t{"Animal"});
  animal[0].format().font_align(FontAlign::center);

  // Animal properties nested table
  Table animal_properties;
  animal_properties.format().width(20);
  animal_properties.add_row(Row_t{"+age: Int"});
  animal_properties.add_row(Row_t{"+gender: String"});
  animal_properties[1].format().hide_border_top();

  // Animal methods nested table
  Table animal_methods;
  animal_methods.format().width(20);
  animal_methods.add_row(Row_t{"+isMammal()"});
  animal_methods.add_row(Row_t{"+mate()"});
  animal_methods[1].format().hide_border_top();

  animal.add_row(Row_t{animal_properties});
  animal.add_row(Row_t{animal_methods});
  animal[2].format().hide_border_top();

  class_diagram.add_row(Row_t{animal});

  // Add rows in the class diagram for the up-facing arrow
  // THanks to center alignment, these will align just fine
  class_diagram.add_row(Row_t{"▲"});
  class_diagram[1][0].format().hide_border_top().multi_byte_characters(true);
  class_diagram.add_row(Row_t{"|"});
  class_diagram[2].format().hide_border_top();
  class_diagram.add_row(Row_t{"|"});
  class_diagram[3].format().hide_border_top();

  // Duck class
  Table duck;
  duck.add_row(Row_t{"Duck"});
  duck[0].format().font_align(FontAlign::center);

  // Duck proeperties nested table
  Table duck_properties;
  duck_properties.format().width(40);
  duck_properties.add_row(Row_t{"+beakColor: String = \"yellow\""});

  // Duck methods nested table
  Table duck_methods;
  duck_methods.format().width(40);
  duck_methods.add_row(Row_t{"+swim()"});
  duck_methods.add_row(Row_t{"+quack()"});
  duck_methods[1].format().hide_border_top();

  duck.add_row(Row_t{duck_properties});
  duck.add_row(Row_t{duck_methods});
  duck[2].format().hide_border_top();

  class_diagram.add_row(Row_t{duck});
  class_diagram[4].format().hide_border_top();

  std::cout << class_diagram << std::endl;
}