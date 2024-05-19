// Copyright 2023 The Elastic-AI Authors.
// part of Elastic AI Search
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//

#pragma once

#include <cstring>
#include <iosfwd>
#include <memory>
#include <string>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <utility>
#include <collie/strings/string_builder.h>
#include <collie/utility/compare.h>
#include <collie/log/logging.h>
#include <collie/base/macros.h>
#include <collie/strings/format.h>

#define COLLIE_RETURN_IF_(condition, status, _) \
  do {                                         \
    if (COLLIE_UNLIKELY(condition)) {      \
      return (status);                         \
    }                                          \
  } while (0)

#define COLLIE_RETURN_IF(condition, status) \
  COLLIE_RETURN_IF_(condition, status, COLLIE_STRINGIFY(status))

/// \brief Propagate any non-successful Status to the caller
#define COLLIE_RETURN_NOT_OK(status)                                   \
  do {                                                                \
    ::collie::Status __s = ::collie::internal::generic_to_status(status); \
    COLLIE_RETURN_IF_(!__s.ok(), __s, COLLIE_STRINGIFY(status));        \
  } while (false)

/// \brief Given `expr` and `warn_msg`; log `warn_msg` if `expr` is a non-ok status
#define COLLIE_WARN_NOT_OK(expr, warn_msg) \
  do {                                    \
    ::collie::Status _s = (expr);          \
    if (COLLIE_UNLIKELY(!_s.ok())) {  \
      _s.warn(warn_msg);                  \
    }                                     \
  } while (false)

#define RETURN_NOT_OK_ELSE(s, else_)                            \
  do {                                                          \
    ::collie::Status _s = ::collie::internal::generic_to_status(s); \
    if (!_s.ok()) {                                             \
      else_;                                                    \
      return _s;                                                \
    }                                                           \
  } while (false)

// This is an internal-use macro and should not be used in public headers.
#ifndef RETURN_NOT_OK
#define RETURN_NOT_OK(s) COLLIE_RETURN_NOT_OK(s)
#endif


namespace collie {

    enum class StatusCode : char {
        OK = 0,
        Cancelled = 1,
        Unknown = 2,
        InvalidArgument = 3,
        DeadlineExceeded = 4,
        NotFound = 5,
        AlreadyExists = 6,
        PermissionDenied = 7,
        ResourceExhausted = 8,
        FailedPrecondition = 9,
        Aborted = 10,
        OutOfRange = 11,
        Unimplemented = 13,
        Internal = 14,
        Unavailable = 15,
        DataLoss = 16,
        Unauthenticated = 17,
        TryAgain = 18,
        AlreadyStopped = 19,
        ResourceBusy = 20,
        IOError = 21
    };

    namespace status_detail {
        inline StatusCode errno_to_status_code(int error_number) {
            switch (error_number) {
                case 0:
                    return StatusCode::OK;
                case EINVAL:        // Invalid argument
                case ENAMETOOLONG:  // Filename too long
                case E2BIG:         // Argument list too long
                case EDESTADDRREQ:  // Destination address required
                case EDOM:          // Mathematics argument out of domain of function
                case EFAULT:        // Bad address
                case EILSEQ:        // Illegal byte sequence
                case ENOPROTOOPT:   // Protocol not available
                case ENOSTR:        // Not a STREAM
                case ENOTSOCK:      // Not a socket
                case ENOTTY:        // Inappropriate I/O control operation
                case EPROTOTYPE:    // Protocol wrong type for socket
                case ESPIPE:        // Invalid seek
                    return StatusCode::InvalidArgument;
                case ETIMEDOUT:  // Connection timed out
                case ETIME:      // Timer expired
                    return StatusCode::DeadlineExceeded;
                case ENODEV:  // No such device
                case ENOENT:  // No such file or directory
#ifdef ENOMEDIUM
                case ENOMEDIUM:  // No medium found
#endif
                case ENXIO:  // No such device or address
                case ESRCH:  // No such process
                    return StatusCode::NotFound;
                case EEXIST:         // File exists
                case EADDRNOTAVAIL:  // Address not available
                case EALREADY:       // Connection already in progress
#ifdef ENOTUNIQ
                case ENOTUNIQ:  // Name not unique on network
#endif
                    return StatusCode::AlreadyExists;
                case EPERM:   // Operation not permitted
                case EACCES:  // Permission denied
#ifdef ENOKEY
                case ENOKEY:  // Required key not available
#endif
                case EROFS:  // Read only file system
                    return StatusCode::PermissionDenied;
                case ENOTEMPTY:   // Directory not empty
                case EISDIR:      // Is a directory
                case ENOTDIR:     // Not a directory
                case EADDRINUSE:  // Address already in use
                case EBADF:       // Invalid file descriptor
#ifdef EBADFD
                case EBADFD:  // File descriptor in bad state
#endif
                case EBUSY:    // Device or resource busy
                case ECHILD:   // No child processes
                case EISCONN:  // Socket is connected
#ifdef EISNAM
                case EISNAM:  // Is a named type file
#endif
#ifdef ENOTBLK
                case ENOTBLK:  // Block device required
#endif
                case ENOTCONN:  // The socket is not connected
                case EPIPE:     // Broken pipe
#ifdef ESHUTDOWN
                case ESHUTDOWN:  // Cannot send after transport endpoint shutdown
#endif
                case ETXTBSY:  // Text file busy
#ifdef EUNATCH
                case EUNATCH:  // Protocol driver not attached
#endif
                    return StatusCode::FailedPrecondition;
                case ENOSPC:  // No space left on device
#ifdef EDQUOT
                case EDQUOT:  // Disk quota exceeded
#endif
                case EMFILE:   // Too many open files
                case EMLINK:   // Too many links
                case ENFILE:   // Too many open files in system
                case ENOBUFS:  // No buffer space available
                case ENODATA:  // No message is available on the STREAM read queue
                case ENOMEM:   // Not enough space
                case ENOSR:    // No STREAM resources
#ifdef EUSERS
                case EUSERS:  // Too many users
#endif
                    return StatusCode::ResourceExhausted;
#ifdef ECHRNG
                case ECHRNG:  // Channel number out of range
#endif
                case EFBIG:      // File too large
                case EOVERFLOW:  // Value too large to be stored in data type
                case ERANGE:     // Result too large
                    return StatusCode::OutOfRange;
#ifdef ENOPKG
                case ENOPKG:  // Package not installed
#endif
                case ENOSYS:        // Function not implemented
                case ENOTSUP:       // Operation not supported
                case EAFNOSUPPORT:  // Address family not supported
#ifdef EPFNOSUPPORT
                case EPFNOSUPPORT:  // Protocol family not supported
#endif
                case EPROTONOSUPPORT:  // Protocol not supported
#ifdef ESOCKTNOSUPPORT
                case ESOCKTNOSUPPORT:  // Socket type not supported
#endif
                case EXDEV:  // Improper link
                    return StatusCode::Unimplemented;
                case EAGAIN:  // Resource temporarily unavailable
#ifdef ECOMM
                case ECOMM:  // Communication error on send
#endif
                case ECONNREFUSED:  // Connection refused
                case ECONNABORTED:  // Connection aborted
                case ECONNRESET:    // Connection reset
                case EINTR:         // Interrupted function call
#ifdef EHOSTDOWN
                case EHOSTDOWN:  // Host is down
#endif
                case EHOSTUNREACH:  // Host is unreachable
                case ENETDOWN:      // Network is down
                case ENETRESET:     // Connection aborted by network
                case ENETUNREACH:   // Network unreachable
                case ENOLCK:        // No locks available
                case ENOLINK:       // Link has been severed
#ifdef ENONET
                case ENONET:  // Machine is not on the network
#endif
                    return StatusCode::Unavailable;
                case EDEADLK:  // Resource deadlock avoided
#ifdef ESTALE
                case ESTALE:  // Stale file handle
#endif
                    return StatusCode::Aborted;
                case ECANCELED:  // Operation cancelled
                    return StatusCode::Cancelled;
                default:
                    return StatusCode::Unknown;
            }
        }
    }  // namespace status_detail

    /// \brief An opaque class that allows subsystems to retain
    /// additional information inside the Status.
    class StatusDetail {
    public:
        virtual ~StatusDetail() = default;

        /// \brief Return a unique id for the type of the StatusDetail
        /// (effectively a poor man's substitute for RTTI).
        [[nodiscard]] virtual const char *type_id() const = 0;

        /// \brief Produce a human-readable description of this status.
        [[nodiscard]] virtual std::string to_string() const = 0;

        bool operator==(const StatusDetail &other) const noexcept {
            return std::string(type_id()) == other.type_id() && to_string() == other.to_string();
        }
    };

    /// \brief Status outcome object (success or error)
    ///
    /// The Status object is an object holding the outcome of an operation.
    /// The outcome is represented as a StatusCode, either success
    /// (StatusCode::OK) or an error (any other of the StatusCode enumeration values).
    ///
    /// Additionally, if an error occurred, a specific error message is generally
    /// attached.
    class [[nodiscard]] Status : public EqualityComparable<Status>, public ToStringOstreamable<Status> {
    public:
        // Create a success status.
        constexpr Status() noexcept: state_(nullptr) {}

        ~Status() noexcept {
            if (COLLIE_UNLIKELY(state_ != nullptr)) {
                DeleteState();
            }
        }

        Status(StatusCode code, const std::string &msg): Status::Status(code, msg, nullptr) {}

        /// \brief Pluggable constructor for use by sub-systems.  detail cannot be null.
        Status(StatusCode code, std::string msg, std::shared_ptr<StatusDetail> detail) {
                CHECK(code != StatusCode::OK) << "Cannot construct ok status with message";
                state_ = new State;
                state_->code = code;
                state_->msg = std::move(msg);
                if (detail != nullptr) {
                    state_->detail = std::move(detail);
                }
        }

        // Copy the specified status.
        inline Status(const Status &s);

        inline Status &operator=(const Status &s);

        // Move the specified status.
        inline Status(Status &&s) noexcept;

        inline Status &operator=(Status &&s) noexcept;

        [[nodiscard]] inline bool equals(const Status &s) const;

        // AND the statuses.
        inline Status operator&(const Status &s) const noexcept;

        inline Status operator&(Status &&s) const noexcept;

        inline Status &operator&=(const Status &s) noexcept;

        inline Status &operator&=(Status &&s) noexcept;

        /// Return a success status
        static Status ok_status() { return Status(); }

        template<typename... Args>
        static Status from_stream_args(StatusCode code, Args &&... args) {
            return Status(code, StringBuilder(std::forward<Args>(args)...));
        }

        template<typename ...Args>
        static Status from_args(StatusCode code, std::string_view fmt, Args &&...args) {
            return Status(code, std::move(format(fmt, std::forward<Args>(args)...)), nullptr);
        }

        template<typename... Args>
        static Status from_detail_and_args(StatusCode code, std::shared_ptr<StatusDetail> detail,
                                        Args &&... args) {
            return Status(code, StringBuilder(std::forward<Args>(args)...),
                          std::move(detail));
        }

        /// Return an error status for errno values
        static Status from_errno(int error_number = -1) {
            if (error_number == -1) {
                error_number = errno;
            }
            return Status::from_args(status_detail::errno_to_status_code(error_number), strerror(error_number));
        }

        template<typename ... Args>
        static Status from_errno(int error_number, std::string_view fmt, Args &&... args) {
            return Status::from_args(status_detail::errno_to_status_code(error_number), fmt, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        static Status from_errno(int error_number, std::shared_ptr<StatusDetail> detail, std::string_view fmt, Args &&... args) {
            if (error_number == -1) {
                error_number = errno;
            }
            return Status::from_detail_and_args(status_detail::errno_to_status_code(error_number), std::move(detail), fmt, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        static Status from_errno(std::shared_ptr<StatusDetail> detail, std::string_view fmt, Args &&... args) {
            return Status::from_detail_and_args(status_detail::errno_to_status_code(errno), std::move(detail), fmt, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        static Status from_errno(std::string_view fmt, Args &&... args) {
            return Status::from_errno(errno, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for cancelled operations
        template<typename... Args>
        static Status canceled(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::Cancelled, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for unknown errors
        template<typename... Args>
        static Status unknown(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::Unknown, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for invalid arguments
        template<typename... Args>
        static Status invalid_argument(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::InvalidArgument, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for deadline exceeded
        template<typename... Args>
        static Status deadline_exceeded(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::DeadlineExceeded, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for not found
        template<typename... Args>
        static Status not_found(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::NotFound, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for already exists
        template<typename... Args>
        static Status already_exists(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::AlreadyExists, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for permission denied
        template<typename... Args>
        static Status permission_denied(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::PermissionDenied, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for resource exhausted
        template<typename... Args>
        static Status resource_exhausted(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::ResourceExhausted, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for failed precondition
        template<typename... Args>
        static Status failed_precondition(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::FailedPrecondition, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for aborted
        template<typename... Args>
        static Status aborted(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::Aborted, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for out of range
        template<typename... Args>
        static Status out_of_range(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::OutOfRange, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for unimplemented
        template<typename... Args>
        static Status unimplemented(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::Unimplemented, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for internal
        template<typename... Args>
        static Status internal(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::Internal, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for unavailable
        template<typename... Args>
        static Status unavailable(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::Unavailable, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for data loss
        template<typename... Args>
        static Status data_loss(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::DataLoss, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for unauthenticated
        template<typename... Args>
        static Status unauthenticated(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::Unauthenticated, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for try again
        template<typename... Args>
        static Status try_again(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::TryAgain, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for already stopped
        template<typename... Args>
        static Status already_stopped(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::AlreadyStopped, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for resource busy
        template<typename... Args>
        static Status resource_busy(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::ResourceBusy, fmt, std::forward<Args>(args)...);
        }

        /// Return an error status for IO error
        template<typename... Args>
        static Status io_error(std::string_view fmt, Args &&... args) {
            return Status::from_args(StatusCode::IOError, fmt, std::forward<Args>(args)...);
        }

        /// Return true iff the status indicates success.
        [[nodiscard]] constexpr bool ok() const { return (state_ == nullptr); }

        /// Return true iff the status indicates an cancellation.
        [[nodiscard]] constexpr bool is_cancelled() const { return code() == StatusCode::Cancelled; }

        /// Return true iff the status indicates an unknown error.
        [[nodiscard]] constexpr bool is_unknown() const { return code() == StatusCode::Unknown; }

        /// Return true iff the status indicates an invalid argument.
        [[nodiscard]] constexpr bool is_invalid_argument() const { return code() == StatusCode::InvalidArgument; }

        /// Return true iff the status indicates a deadline exceeded.
        [[nodiscard]] constexpr bool is_deadline_exceeded() const { return code() == StatusCode::DeadlineExceeded; }


        /// Return true iff the status indicates a not found error.
        [[nodiscard]] constexpr bool is_not_found() const { return code() == StatusCode::NotFound; }

        /// Return true iff the status indicates an already exists error.
        [[nodiscard]] constexpr bool is_already_exists() const { return code() == StatusCode::AlreadyExists; }

        /// Return true iff the status indicates a permission denied error.
        [[nodiscard]] constexpr bool is_permission_denied() const { return code() == StatusCode::PermissionDenied; }

        /// Return true iff the status indicates a resource exhausted error.
        [[nodiscard]] constexpr bool is_resource_exhausted() const { return code() == StatusCode::ResourceExhausted; }


        /// Return true iff the status indicates a failed precondition.
        [[nodiscard]] constexpr bool is_failed_precondition() const { return code() == StatusCode::FailedPrecondition; }

        /// Return true iff the status indicates an aborted operation.
        [[nodiscard]] constexpr bool is_aborted() const { return code() == StatusCode::Aborted; }

        /// Return true iff the status indicates an out-of-range error.
        [[nodiscard]] constexpr bool is_out_of_range() const { return code() == StatusCode::OutOfRange; }

        /// Return true iff the status indicates an unimplemented operation.
        [[nodiscard]] constexpr bool is_unimplemented() const { return code() == StatusCode::Unimplemented; }

        /// Return true iff the status indicates an internal error.
        [[nodiscard]] constexpr bool is_internal() const { return code() == StatusCode::Internal; }

        /// Return true iff the status indicates an unavailable error.
        [[nodiscard]] constexpr bool is_unavailable() const { return code() == StatusCode::Unavailable; }

        /// Return true iff the status indicates a data loss error.
        [[nodiscard]] constexpr bool is_data_loss() const { return code() == StatusCode::DataLoss; }

        /// Return true iff the status indicates an unauthenticated error.
        [[nodiscard]] constexpr bool is_unauthenticated() const { return code() == StatusCode::Unauthenticated; }

        /// Return true iff the status indicates a try again error.
        [[nodiscard]] constexpr bool is_try_again() const { return code() == StatusCode::TryAgain; }

        /// Return true iff the status indicates an already stopped error.
        [[nodiscard]] constexpr bool is_already_stopped() const { return code() == StatusCode::AlreadyStopped; }

        /// Return true iff the status indicates a resource busy error.
        [[nodiscard]] constexpr bool is_resource_busy() const { return code() == StatusCode::ResourceBusy; }

        /// Return true iff the status indicates an IO error.
        [[nodiscard]] constexpr bool is_io_error() const { return code() == StatusCode::IOError; }

        /// \brief Return a string representation of this status suitable for printing.
        ///
        /// The string "OK" is returned for success.
        [[nodiscard]] std::string to_string() const {
            std::string result(code_as_string());
            if (state_ == nullptr) {
                return result;
            }
            result += ": ";
            result += state_->msg;
            if (state_->detail != nullptr) {
                result += ". Detail: ";
                result += state_->detail->to_string();
            }

            return result;
        }

        /// \brief Return a string representation of the status code, without the message
        /// text or POSIX code information.
        [[nodiscard]] std::string code_as_string() const {
            if (state_ == nullptr) {
                return "OK";
            }
            return code_as_string(code());
        }

        static std::string code_as_string(StatusCode code) {
            const char* type;
            switch (code) {
                case StatusCode::OK:
                    type = "OK";
                    break;
                case StatusCode::Cancelled:
                    type = "CANCELLED";
                    break;
                case StatusCode::Unknown:
                    type = "UNKNOWN";
                    break;
                case StatusCode::InvalidArgument:
                    type = "INVALID_ARGUMENT";
                    break;
                case StatusCode::DeadlineExceeded:
                    type = "DEADLINE_EXCEEDED";
                    break;
                case StatusCode::NotFound:
                    type = "NOT_FOUND";
                    break;
                case StatusCode::AlreadyExists:
                    type = "ALREADY_EXISTS";
                    break;
                case StatusCode::PermissionDenied:
                    type = "PERMISSION_DENIED";
                    break;
                case StatusCode::ResourceExhausted:
                    type = "RESOURCE_EXHAUSTED";
                    break;
                case StatusCode::FailedPrecondition:
                    type = "FAILED_PRECONDITION";
                    break;
                case StatusCode::Aborted:
                    type = "ABORTED";
                    break;
                case StatusCode::OutOfRange:
                    type = "OUT_OF_RANGE";
                    break;
                case StatusCode::Unimplemented:
                    type = "UNIMPLEMENTED";
                    break;
                case StatusCode::Internal:
                    type = "INTERNAL";
                    break;
                case StatusCode::Unavailable:
                    type = "UNAVAILABLE";
                    break;
                case StatusCode::DataLoss:
                    type = "DATA_LOSS";
                    break;
                case StatusCode::Unauthenticated:
                    type = "UNAUTHENTICATED";
                    break;
                case StatusCode::TryAgain:
                    type = "TRY_AGAIN";
                    break;
                case StatusCode::AlreadyStopped:
                    type = "ALREADY_STOPPED";
                    break;
                case StatusCode::ResourceBusy:
                    type = "RESOURCE_BUSY";
                    break;
                case StatusCode::IOError:
                    type = "IO_ERROR";
                    break;
                default:
                    type = "Unknown";
                    break;
            }
            return std::string(type);
        }

        /// \brief Return the StatusCode value attached to this status.
        [[nodiscard]] constexpr StatusCode code() const { return ok() ? StatusCode::OK : state_->code; }

        /// \brief Return the specific error message attached to this status.
        [[nodiscard]] const std::string &message() const {
            static const std::string no_message = "";
            return ok() ? no_message : state_->msg;
        }

        /// \brief Return the status detail attached to this message.
        [[nodiscard]] const std::shared_ptr<StatusDetail> &detail() const {
            static std::shared_ptr<StatusDetail> no_detail = nullptr;
            return state_ ? state_->detail : no_detail;
        }

        /// \brief Return a new Status copying the existing status, but
        /// updating with the existing detail.
        [[nodiscard]] Status with_detail(std::shared_ptr<StatusDetail> new_detail) const {
            return Status(code(), message(), std::move(new_detail));
        }

        /// \brief Return a new Status with changed message, copying the
        /// existing status code and detail.
        template<typename... Args>
        Status WithMessage(Args &&... args) const {
            return FromArgs(code(), std::forward<Args>(args)...).WithDetail(detail());
        }

        void warn() const {
            LOG(WARN) << to_string();
        }

        void warn(const std::string &message) const {
            LOG(WARN) << message << ": " << to_string();
        }

        [[noreturn]] void abort() const {
            abort(std::string());
        }

        [[noreturn]] void abort(const std::string &message) const {
            std::cerr << "-- Collie Fatal Error --\n";
            if (!message.empty()) {
                std::cerr << message << "\n";
            }
            std::cerr << to_string() << std::endl;
            std::abort();
        }

    private:
        struct State {
            StatusCode code;
            std::string msg;
            std::shared_ptr<StatusDetail> detail;
        };
        // OK status has a `NULL` state_.  Otherwise, `state_` points to
        // a `State` structure containing the error code and message(s)
        State *state_;

        void DeleteState() {
            delete state_;
            state_ = nullptr;
        }

        void CopyFrom(const Status &s) {
            delete state_;
            if (s.state_ == nullptr) {
                state_ = nullptr;
            } else {
                state_ = new State(*s.state_);
            }
        }

        inline void MoveFrom(Status &s);
    };

    void Status::MoveFrom(Status &s) {
        delete state_;
        state_ = s.state_;
        s.state_ = nullptr;
    }

    Status::Status(const Status &s)
            : state_((s.state_ == nullptr) ? nullptr : new State(*s.state_)) {}

    Status &Status::operator=(const Status &s) {
        // The following condition catches both aliasing (when this == &s),
        // and the common case where both s and *this are ok.
        if (state_ != s.state_) {
            CopyFrom(s);
        }
        return *this;
    }

    Status::Status(Status &&s) noexcept: state_(s.state_) { s.state_ = nullptr; }

    Status &Status::operator=(Status &&s) noexcept {
        MoveFrom(s);
        return *this;
    }

    bool Status::equals(const Status &s) const {
        if (state_ == s.state_) {
            return true;
        }

        if (ok() || s.ok()) {
            return false;
        }

        if (detail() != s.detail()) {
            if ((detail() && !s.detail()) || (!detail() && s.detail())) {
                return false;
            }
            return *detail() == *s.detail();
        }

        return code() == s.code() && message() == s.message();
    }

    /// \cond FALSE
    // (note: emits warnings on Doxygen < 1.8.15,
    //  see https://github.com/doxygen/doxygen/issues/6295)
    Status Status::operator&(const Status &s) const noexcept {
        if (ok()) {
            return s;
        } else {
            return *this;
        }
    }

    Status Status::operator&(Status &&s) const noexcept {
        if (ok()) {
            return std::move(s);
        } else {
            return *this;
        }
    }

    Status &Status::operator&=(const Status &s) noexcept {
        if (ok() && !s.ok()) {
            CopyFrom(s);
        }
        return *this;
    }

    Status &Status::operator&=(Status &&s) noexcept {
        if (ok() && !s.ok()) {
            MoveFrom(s);
        }
        return *this;
    }
    /// \endcond

    namespace internal {

        // Extract Status from Status or Result<T>
        // Useful for the status check macros such as RETURN_NOT_OK.
        inline const Status &generic_to_status(const Status &st) { return st; }

        inline Status generic_to_status(Status &&st) { return std::move(st); }

    }  // namespace internal

}  // namespace collie

namespace fmt {
    template<>
    struct formatter<collie::Status>: public formatter<std::string> {
        template<typename FormatContext>
        auto format(const collie::Status &s, FormatContext &ctx) {
            formatter<std::string>::format(s.to_string(), ctx);
        }
    };
}  // namespace fmt
