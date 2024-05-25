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

#include "subcommand_a.hpp"
#include <iostream>
#include <memory>

/// Set up a subcommand and capture a shared_ptr to a struct that holds all its options.
/// The variables of the struct are bound to the CLI options.
/// We use a shared ptr so that the addresses of the variables remain for binding,
/// You could return the shared pointer if you wanted to access the values in main.
void setup_subcommand_a(collie::App &app) {
    // Create the option and subcommand objects.
    auto opt = std::make_shared<SubcommandAOptions>();
    auto *sub = app.add_subcommand("subcommand_a", "performs subcommand a");

    // Add options to sub, binding them to opt.
    sub->add_option("-f,--file", opt->file, "File name")->required();
    sub->add_flag("--with-foo", opt->with_foo, "Counter");

    // Set the run function as callback to be called when this subcommand is issued.
    sub->callback([opt]() { run_subcommand_a(*opt); });
}

/// The function that runs our code.
/// This could also simply be in the callback lambda itself,
/// but having a separate function is cleaner.
void run_subcommand_a(SubcommandAOptions const &opt) {
    // Do stuff...
    std::cout << "Working on file: " << opt.file << '\n';
    if(opt.with_foo) {
        std::cout << "Using foo!" << '\n';
    }
}
