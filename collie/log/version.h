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

#pragma once

#define CLOG_VER_MAJOR 1
#define CLOG_VER_MINOR 13
#define CLOG_VER_PATCH 0

#define CLOG_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define CLOG_VERSION CLOG_TO_VERSION(CLOG_VER_MAJOR, CLOG_VER_MINOR, CLOG_VER_PATCH)
