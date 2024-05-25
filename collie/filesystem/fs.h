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
//

#ifndef COLLIE_FILESYSTEM_FS_H_
#define COLLIE_FILESYSTEM_FS_H_

#include <string_view>
#include <collie/filesystem/ghc/filesystem.hpp>

namespace collie {
    using namespace ghc;

    inline bool has_extension(const std::string_view &filename, const std::string_view &extension) {
        return filename.size() >= extension.size() &&
               filename.compare(filename.size() - extension.size(), extension.size(),
                                extension) == 0;
    }

    inline std::string_view trim_extension(const std::string_view &filename, const std::string_view &extension) {
        if (has_extension(filename, extension)) {
            return filename.substr(0, filename.size() - extension.size());
        } else {
            return filename;
        }
    }

}
#endif  // COLLIE_FILESYSTEM_FS_H_
