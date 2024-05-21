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

#include <collie/cli/cli.h>
#include <iostream>
#include <map>
#include <string>

enum class Level : int { High, Medium, Low };

inline std::ostream &operator<<(std::ostream &os, const Level &level) {
    switch(level) {
    case Level::High:
        os << "High";
        break;
    case Level::Medium:
        os << "Medium";
        break;
    case Level::Low:
        os << "Low";
        break;
    }
    os << " (ft rom custom ostream)";
    return os;
}

int main(int argc, char **argv) {
    collie::App app;

    Level level{Level::Low};
    // specify string->value mappings
    std::map<std::string, Level> map{{"high", Level::High}, {"medium", Level::Medium}, {"low", Level::Low}};
    // CheckedTransformer translates and checks whether the results are either in one of the strings or in one of the
    // translations already
    app.add_option("-l,--level", level, "Level settings")
        ->required()
        ->transform(collie::CheckedTransformer(map, collie::ignore_case));

    COLLIE_CLI_PARSE(app, argc, argv);

    using collie::enums::operator<<;
    std::cout << "Enum received: " << level << '\n';

    return 0;
}
