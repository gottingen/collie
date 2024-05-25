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
#if __cplusplus >= 201703L
#include <string_view>
using std::string_view;
#else
#include <collie/table/string_view_lite.h>
using collie::string_view;
#endif

int main() {
  Table table;

  string_view c0 = "string_view";
  const char *c1 = "const char *";
  std::string c2 = "std::string";

  Table c3;
  c3.add_row({"Table", "", ""});
  c3.add_row({c0, c1, c2});
  c3[0].format().border("").corner("");

  table.add_row({c0, c1, c2, c3});

  std::cout << table << std::endl;
}