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

#include <collie/log/common.h>
#include <tuple>

namespace collie::log {
    namespace details {

// Helper class for file sinks.
// When failing to open a file, retry several times(5) with a delay interval(10 ms).
// Throw CLogEx exception on errors.

        class file_helper {
        public:
            file_helper() = default;

            explicit file_helper(const file_event_handlers &event_handlers);

            file_helper(const file_helper &) = delete;

            file_helper &operator=(const file_helper &) = delete;

            ~file_helper();

            void open(const filename_t &fname, bool truncate = false);

            void reopen(bool truncate);

            void flush();

            void sync();

            void close();

            void write(const memory_buf_t &buf);

            size_t size() const;

            const filename_t &filename() const;

            //
            // return file path and its extension:
            //
            // "mylog.txt" => ("mylog", ".txt")
            // "mylog" => ("mylog", "")
            // "mylog." => ("mylog.", "")
            // "/dir1/dir2/mylog.txt" => ("/dir1/dir2/mylog", ".txt")
            //
            // the starting dot in filenames is ignored (hidden files):
            //
            // ".mylog" => (".mylog". "")
            // "my_folder/.mylog" => ("my_folder/.mylog", "")
            // "my_folder/.mylog.txt" => ("my_folder/.mylog", ".txt")
            static std::tuple<filename_t, filename_t> split_by_extension(const filename_t &fname);

        private:
            const int open_tries_ = 5;
            const unsigned int open_interval_ = 10;
            std::FILE *fd_{nullptr};
            filename_t filename_;
            file_event_handlers event_handlers_;
        };
    }  // namespace details
}  // namespace collie::log

#include <collie/log/details/file_helper-inl.h>
