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
#include <string>

int main(int argc, char **argv) {

    collie::App app("test for positional arity");
    std::string lib;
    app.add_option("-l, --lib", "list all the files");
    std::vector<std::string> files;
    app.add_option("file1", files, "first file")->required();

    COLLIE_CLI_PARSE(app, argc, argv);
    for(auto &file : files) {
        std::cout << "File = " << file << '\n';
    }
    return 0;
}
