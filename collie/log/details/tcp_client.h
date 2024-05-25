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

#ifdef _WIN32
    #error include tcp_client-windows.h instead
#endif

// tcp client helper
#include <collie/log/common.h>
#include <collie/log/details/os.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

namespace collie::log {
namespace details {
class tcp_client {
    int socket_ = -1;

public:
    bool is_connected() const { return socket_ != -1; }

    void close() {
        if (is_connected()) {
            ::close(socket_);
            socket_ = -1;
        }
    }

    int fd() const { return socket_; }

    ~tcp_client() { close(); }

    // try to connect or throw on failure
    void connect(const std::string &host, int port) {
        close();
        struct addrinfo hints {};
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;      // To work with IPv4, IPv6, and so on
        hints.ai_socktype = SOCK_STREAM;  // TCP
        hints.ai_flags = AI_NUMERICSERV;  // port passed as as numeric value
        hints.ai_protocol = 0;

        auto port_str = std::to_string(port);
        struct addrinfo *addrinfo_result;
        auto rv = ::getaddrinfo(host.c_str(), port_str.c_str(), &hints, &addrinfo_result);
        if (rv != 0) {
            throw_clog_ex(fmt_lib::format("::getaddrinfo failed: {}", gai_strerror(rv)));
        }

        // Try each address until we successfully connect(2).
        int last_errno = 0;
        for (auto *rp = addrinfo_result; rp != nullptr; rp = rp->ai_next) {
#if defined(SOCK_CLOEXEC)
            const int flags = SOCK_CLOEXEC;
#else
            const int flags = 0;
#endif
            socket_ = ::socket(rp->ai_family, rp->ai_socktype | flags, rp->ai_protocol);
            if (socket_ == -1) {
                last_errno = errno;
                continue;
            }
            rv = ::connect(socket_, rp->ai_addr, rp->ai_addrlen);
            if (rv == 0) {
                break;
            }
            last_errno = errno;
            ::close(socket_);
            socket_ = -1;
        }
        ::freeaddrinfo(addrinfo_result);
        if (socket_ == -1) {
            throw_clog_ex("::connect failed", last_errno);
        }

        // set TCP_NODELAY
        int enable_flag = 1;
        ::setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&enable_flag),
                     sizeof(enable_flag));

        // prevent sigpipe on systems where MSG_NOSIGNAL is not available
#if defined(SO_NOSIGPIPE) && !defined(MSG_NOSIGNAL)
        ::setsockopt(socket_, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<char *>(&enable_flag),
                     sizeof(enable_flag));
#endif

#if !defined(SO_NOSIGPIPE) && !defined(MSG_NOSIGNAL)
    #error "tcp_sink would raise SIGPIPE since neither SO_NOSIGPIPE nor MSG_NOSIGNAL are available"
#endif
    }

    // Send exactly n_bytes of the given data.
    // On error close the connection and throw.
    void send(const char *data, size_t n_bytes) {
        size_t bytes_sent = 0;
        while (bytes_sent < n_bytes) {
#if defined(MSG_NOSIGNAL)
            const int send_flags = MSG_NOSIGNAL;
#else
            const int send_flags = 0;
#endif
            auto write_result =
                ::send(socket_, data + bytes_sent, n_bytes - bytes_sent, send_flags);
            if (write_result < 0) {
                close();
                throw_clog_ex("write(2) failed", errno);
            }

            if (write_result == 0)  // (probably should not happen but in any case..)
            {
                break;
            }
            bytes_sent += static_cast<size_t>(write_result);
        }
    }
};
}  // namespace details
}  // namespace collie::log
