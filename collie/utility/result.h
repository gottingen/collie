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
#include <cstddef>
#include <new>
#include <string>
#include <type_traits>
#include <utility>

#include <collie/utility/status.h>
#include <collie/memory/aligned_storage.h>
#include <collie/utility/compare.h>

namespace collie {

    template <typename>
    struct ensure_result;

    namespace internal {

        inline void die_with_message(const std::string& msg) {
            LOG(FATAL) << msg;
        }

        inline void invalid_value_or_die(const Status& st) {
            die_with_message(std::string("ValueOrDie called on an error: ") + st.to_string());
        }

    }  // namespace internal

    /// A class for representing either a usable value, or an error.
    ///
    /// A Result object either contains a value of type `T` or a Status object
    /// explaining why such a value is not present. The type `T` must be
    /// copy-constructible and/or move-constructible.
    ///
    /// The state of a Result object may be determined by calling ok() or
    /// status(). The ok() method returns true if the object contains a valid value.
    /// The status() method returns the internal Status object. A Result object
    /// that contains a valid value will return an OK Status for a call to status().
    ///
    /// A value of type `T` may be extracted from a Result object through a call
    /// to value_or_die(). This function should only be called if a call to ok()
    /// returns true. Sample usage:
    ///
    /// ```
    ///   collie::Result<Foo> result = CalculateFoo();
    ///   if (result.ok()) {
    ///     Foo foo = result.value_or_die();
    ///     foo.DoSomethingCool();
    ///   } else {
    ///     LOG(ERROR) << result.status();
    ///  }
    /// ```
    ///
    /// If `T` is a move-only type, like `std::unique_ptr<>`, then the value should
    /// only be extracted after invoking `std::move()` on the Result object.
    /// Sample usage:
    ///
    /// ```
    ///   collie::Result<std::unique_ptr<Foo>> result = CalculateFoo();
    ///   if (result.ok()) {
    ///     std::unique_ptr<Foo> foo = std::move(result).value_or_die();
    ///     foo->DoSomethingCool();
    ///   } else {
    ///     LOG(ERROR) << result.status();
    ///   }
    /// ```
    ///
    /// Result is provided for the convenience of implementing functions that
    /// return some value but may fail during execution. For instance, consider a
    /// function with the following signature:
    ///
    /// ```
    ///   collie::Status CalculateFoo(int *output);
    /// ```
    ///
    /// This function may instead be written as:
    ///
    /// ```
    ///   collie::Result<int> CalculateFoo();
    /// ```
    template <class T>
    class [[nodiscard]] Result : public EqualityComparable<Result<T>> {
        template <typename U>
        friend class Result;

        static_assert(!std::is_same<T, Status>::value,
                      "this assert indicates you have probably made a metaprogramming error");

    public:
        using ValueType = T;

        /// Constructs a Result object that contains a non-OK status.
        ///
        /// This constructor is marked `explicit` to prevent attempts to `return {}`
        /// from a function with a return type of, for example,
        /// `Result<std::vector<int>>`. While `return {}` seems like it would return
        /// an empty vector, it will actually invoke the default constructor of
        /// Result.
        explicit Result() noexcept  // NOLINT(runtime/explicit)
                : status_(Status::unknown("Uninitialized Result<T>")) {}

        ~Result() noexcept { Destroy(); }

        /// Constructs a Result object with the given non-OK Status object. All
        /// calls to value_or_die() on this object will abort. The given `status` must
        /// not be an OK status, otherwise this constructor will abort.
        ///
        /// This constructor is not declared explicit so that a function with a return
        /// type of `Result<T>` can return a Status object, and the status will be
        /// implicitly converted to the appropriate return type as a matter of
        /// convenience.
        ///
        /// \param status The non-OK Status object to initialize to.
        Result(const Status& status) noexcept  // NOLINT(runtime/explicit)
                : status_(status) {
            if (COLLIE_UNLIKELY(status.ok())) {
                internal::die_with_message(std::string("Constructed with a non-error status: ") +
                                         status.to_string());
            }
        }

        /// Constructs a Result object that contains `value`. The resulting object
        /// is considered to have an OK status. The wrapped element can be accessed
        /// with value_or_die().
        ///
        /// This constructor is made implicit so that a function with a return type of
        /// `Result<T>` can return an object of type `U &&`, implicitly converting
        /// it to a `Result<T>` object.
        ///
        /// Note that `T` must be implicitly constructible from `U`, and `U` must not
        /// be a (cv-qualified) Status or Status-reference type. Due to C++
        /// reference-collapsing rules and perfect-forwarding semantics, this
        /// constructor matches invocations that pass `value` either as a const
        /// reference or as an rvalue reference. Since Result needs to work for both
        /// reference and rvalue-reference types, the constructor uses perfect
        /// forwarding to avoid invalidating arguments that were passed by reference.
        /// See http://thbecker.net/articles/rvalue_references/section_08.html for
        /// additional details.
        ///
        /// \param value The value to initialize to.
        template <typename U,
                typename E = typename std::enable_if<
                        std::is_constructible<T, U>::value && std::is_convertible<U, T>::value &&
                        !std::is_same<typename std::remove_reference<
                                typename std::remove_cv<U>::type>::type,
                                Status>::value>::type>
        Result(U&& value) noexcept {  // NOLINT(runtime/explicit)
            construct_value(std::forward<U>(value));
        }

        /// Constructs a Result object that contains `value`. The resulting object
        /// is considered to have an OK status. The wrapped element can be accessed
        /// with value_or_die().
        ///
        /// This constructor is made implicit so that a function with a return type of
        /// `Result<T>` can return an object of type `T`, implicitly converting
        /// it to a `Result<T>` object.
        ///
        /// \param value The value to initialize to.
        // NOTE `Result(U&& value)` above should be sufficient, but some compilers
        // fail matching it.
        Result(T&& value) noexcept {  // NOLINT(runtime/explicit)
            construct_value(std::move(value));
        }

        /// Copy constructor.
        ///
        /// This constructor needs to be explicitly defined because the presence of
        /// the move-assignment operator deletes the default copy constructor. In such
        /// a scenario, since the deleted copy constructor has stricter binding rules
        /// than the templated copy constructor, the templated constructor cannot act
        /// as a copy constructor, and any attempt to copy-construct a `Result`
        /// object results in a compilation error.
        ///
        /// \param other The value to copy from.
        Result(const Result& other) noexcept : status_(other.status_) {
            if (COLLIE_LIKELY(status_.ok())) {
                construct_value(other.value_unsafe());
            }
        }

        /// Templatized constructor that constructs a `Result<T>` from a const
        /// reference to a `Result<U>`.
        ///
        /// `T` must be implicitly constructible from `const U &`.
        ///
        /// \param other The value to copy from.
        template <typename U, typename E = typename std::enable_if<
                std::is_constructible<T, const U&>::value &&
                std::is_convertible<U, T>::value>::type>
        Result(const Result<U>& other) noexcept : status_(other.status_) {
            if (COLLIE_LIKELY(status_.ok())) {
                construct_value(other.value_unsafe());
            }
        }

        /// Copy-assignment operator.
        ///
        /// \param other The Result object to copy.
        Result& operator=(const Result& other) noexcept {
            // Check for self-assignment.
            if (COLLIE_UNLIKELY(this == &other)) {
                return *this;
            }
            Destroy();
            status_ = other.status_;
            if (COLLIE_LIKELY(status_.ok())) {
                construct_value(other.value_unsafe());
            }
            return *this;
        }

        /// Templatized constructor which constructs a `Result<T>` by moving the
        /// contents of a `Result<U>`. `T` must be implicitly constructible from `U
        /// &&`.
        ///
        /// Sets `other` to contain a non-OK status with a`StatusError::Invalid`
        /// error code.
        ///
        /// \param other The Result object to move from and set to a non-OK status.
        template <typename U,
                typename E = typename std::enable_if<std::is_constructible<T, U&&>::value &&
                                                     std::is_convertible<U, T>::value>::type>
        Result(Result<U>&& other) noexcept {
            if (COLLIE_LIKELY(other.status_.ok())) {
                status_ = std::move(other.status_);
                construct_value(other.move_value_unsafe());
            } else {
                // If we moved the status, the other status may become ok but the other
                // value hasn't been constructed => crash on other destructor.
                status_ = other.status_;
            }
        }

        /// Move-assignment operator.
        ///
        /// Sets `other` to an invalid state..
        ///
        /// \param other The Result object to assign from and set to a non-OK
        /// status.
        Result& operator=(Result&& other) noexcept {
            // Check for self-assignment.
            if (COLLIE_UNLIKELY(this == &other)) {
                return *this;
            }
            Destroy();
            if (COLLIE_LIKELY(other.status_.ok())) {
                status_ = std::move(other.status_);
                construct_value(other.move_value_unsafe());
            } else {
                // If we moved the status, the other status may become ok but the other
                // value hasn't been constructed => crash on other destructor.
                status_ = other.status_;
            }
            return *this;
        }

        /// Compare to another Result.
        bool equals(const Result& other) const {
            if (COLLIE_LIKELY(status_.ok())) {
                return other.status_.ok() && value_unsafe() == other.value_unsafe();
            }
            return status_ == other.status_;
        }

        /// Indicates whether the object contains a `T` value.  Generally instead
        /// of accessing this directly you will want to use ASSIGN_OR_RAISE defined
        /// below.
        ///
        /// \return True if this Result object's status is OK (i.e. a call to ok()
        /// returns true). If this function returns true, then it is safe to access
        /// the wrapped element through a call to value_or_die().
        constexpr bool ok() const { return status_.ok(); }

        /// \brief Equivalent to ok().
        // operator bool() const { return ok(); }

        /// Gets the stored status object, or an OK status if a `T` value is stored.
        ///
        /// \return The stored non-OK status object, or an OK status if this object
        ///         has a value.
        constexpr const Status& status() const { return status_; }

        /// Gets the stored `T` value.
        ///
        /// This method should only be called if this Result object's status is OK
        /// (i.e. a call to ok() returns true), otherwise this call will abort.
        ///
        /// \return The stored `T` value.
        const T& value_or_die() const& {
            if (COLLIE_UNLIKELY(!ok())) {
                internal::invalid_value_or_die(status_);
            }
            return value_unsafe();
        }
        const T& operator*() const& { return value_or_die(); }
        const T* operator->() const { return &value_or_die(); }

        /// Gets a mutable reference to the stored `T` value.
        ///
        /// This method should only be called if this Result object's status is OK
        /// (i.e. a call to ok() returns true), otherwise this call will abort.
        ///
        /// \return The stored `T` value.
        T& value_or_die() & {
            if (COLLIE_UNLIKELY(!ok())) {
                internal::invalid_value_or_die(status_);
            }
            return value_unsafe();
        }
        T& operator*() & { return value_or_die(); }
        T* operator->() { return &value_or_die(); }

        /// Moves and returns the internally-stored `T` value.
        ///
        /// This method should only be called if this Result object's status is OK
        /// (i.e. a call to ok() returns true), otherwise this call will abort. The
        /// Result object is invalidated after this call and will be updated to
        /// contain a non-OK status.
        ///
        /// \return The stored `T` value.
        T value_or_die() && {
            if (COLLIE_UNLIKELY(!ok())) {
                internal::invalid_value_or_die(status_);
            }
            return move_value_unsafe();
        }
        T operator*() && { return std::move(*this).value_or_die(); }

        /// Helper method for implementing Status returning functions in terms of semantically
        /// equivalent Result returning functions. For example:
        ///
        /// Status GetInt(int *out) { return GetInt().Value(out); }
        template <typename U, typename E = typename std::enable_if<
                std::is_constructible<U, T>::value>::type>
        Status value(U* out) && {
            if (!ok()) {
                return status();
            }
            *out = U(move_value_unsafe());
            return Status::ok_status();
        }

        /// Move and return the internally stored value or alternative if an error is stored.
        T value_or(T alternative) && {
            if (!ok()) {
                return alternative;
            }
            return move_value_unsafe();
        }

        /// Retrieve the value if ok(), falling back to an alternative generated by the provided
        /// factory
        template <typename G>
        T value_or_else(G&& generate_alternative) && {
            if (ok()) {
                return move_value_unsafe();
            }
            return std::forward<G>(generate_alternative)();
        }

        /// Apply a function to the internally stored value to produce a new result or propagate
        /// the stored error.
        template <typename M>
        typename ensure_result<decltype(std::declval<M&&>()(std::declval<T&&>()))>::type Map(
                M&& m) && {
            if (!ok()) {
                return status();
            }
            return std::forward<M>(m)(move_value_unsafe());
        }

        /// Apply a function to the internally stored value to produce a new result or propagate
        /// the stored error.
        template <typename M>
        typename ensure_result<decltype(std::declval<M&&>()(std::declval<const T&>()))>::type
        Map(M&& m) const& {
            if (!ok()) {
                return status();
            }
            return std::forward<M>(m)(value_unsafe());
        }

        /// Cast the internally stored value to produce a new result or propagate the stored
        /// error.
        template <typename U, typename E = typename std::enable_if<
                std::is_constructible<U, T>::value>::type>
        Result<U> As() && {
            if (!ok()) {
                return status();
            }
            return U(move_value_unsafe());
        }

        /// Cast the internally stored value to produce a new result or propagate the stored
        /// error.
        template <typename U, typename E = typename std::enable_if<
                std::is_constructible<U, const T&>::value>::type>
        Result<U> As() const& {
            if (!ok()) {
                return status();
            }
            return U(value_unsafe());
        }

        constexpr const T& value_unsafe() const& { return *storage_.get(); }

        constexpr T& value_unsafe() & { return *storage_.get(); }

        T value_unsafe() && { return move_value_unsafe(); }

        T move_value_unsafe() { return std::move(*storage_.get()); }

    private:
        Status status_;  // pointer-sized
        internal::AlignedStorage<T> storage_;

        template <typename U>
        void construct_value(U&& u) noexcept {
            storage_.construct(std::forward<U>(u));
        }

        void Destroy() noexcept {
            if (COLLIE_LIKELY(status_.ok())) {
                static_assert(offsetof(Result<T>, status_) == 0,
                              "Status is guaranteed to be at the start of Result<>");
                storage_.destroy();
            }
        }
    };

#define COLLIE_ASSIGN_OR_RETURN_IMPL(result_name, lhs, rexpr)                              \
  auto&& result_name = (rexpr);                                                          \
  COLLIE_RETURN_IF_(!(result_name).ok(), (result_name).status(), COLLIE_STRINGIFY(rexpr)); \
  lhs = std::move(result_name).value_unsafe();

#define COLLIE_ASSIGN_OR_RETURN_NAME(x, y) COLLIE_CONCAT(x, y)

    /// \brief Execute an expression that returns a Result, extracting its value
    /// into the variable defined by `lhs` (or returning a Status on error).
    ///
    /// Example: Assigning to a new value:
    ///   COLLIE_ASSIGN_OR_RETURN(auto value, MaybeGetValue(arg));
    ///
    /// Example: Assigning to an existing value:
    ///   ValueType value;
    ///   COLLIE_ASSIGN_OR_RETURN(value, MaybeGetValue(arg));
    ///
    /// WARNING: COLLIE_ASSIGN_OR_RETURN expands into multiple statements;
    /// it cannot be used in a single statement (e.g. as the body of an if
    /// statement without {})!
    ///
    /// WARNING: COLLIE_ASSIGN_OR_RETURN `std::move`s its right operand. If you have
    /// an lvalue Result which you *don't* want to move out of cast appropriately.
    ///
    /// WARNING: COLLIE_ASSIGN_OR_RETURN is not a single expression; it will not
    /// maintain lifetimes of all temporaries in `rexpr` (e.g.
    /// `COLLIE_ASSIGN_OR_RETURN(auto x, MakeTemp().GetResultRef());`
    /// will most likely segfault)!
#define COLLIE_ASSIGN_OR_RETURN(lhs, rexpr)                                              \
  COLLIE_ASSIGN_OR_RETURN_IMPL(COLLIE_ASSIGN_OR_RETURN_NAME(_error_or_value, __COUNTER__), \
                             lhs, rexpr);

    namespace internal {

        template <typename T>
        inline const Status& generic_to_status(const Result<T>& res) {
            return res.status();
        }

        template <typename T>
        inline Status generic_to_status(Result<T>&& res) {
            return std::move(res).status();
        }

    }  // namespace internal

    template <typename T, typename R = typename ensure_result<T>::type>
    inline R to_result(T t) {
        return R(std::move(t));
    }

    template <typename T>
    struct ensure_result {
        using type = Result<T>;
    };

    template <typename T>
    struct ensure_result<Result<T>> {
        using type = Result<T>;
    };

}  // namespace collie

