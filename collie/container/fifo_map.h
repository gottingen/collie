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

#ifndef COLLIE_CONTAINER_FIFO_MAP_H_
#define COLLIE_CONTAINER_FIFO_MAP_H_

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace collie {

    template<class Key>
    class fifo_map_compare {
    public:
        /// constructor given a pointer to a key storage
        fifo_map_compare(std::unordered_map<Key, std::size_t> *k) : keys(k) {}

        /*!
        This function compares two keys with respect to the order in which they
        were added to the container. For this, the mapping keys is used.
        */
        bool operator()(const Key &lhs, const Key &rhs) const {
            // look up timestamps for both keys
            const auto timestamp_lhs = keys->find(lhs);
            const auto timestamp_rhs = keys->find(rhs);

            if (timestamp_lhs == keys->end()) {
                // timestamp for lhs not found - cannot be smaller than for rhs
                return false;
            }

            if (timestamp_rhs == keys->end()) {
                // timestamp for rhs not found - timestamp for lhs is smaller
                return true;
            }

            // compare timestamps
            return timestamp_lhs->second < timestamp_rhs->second;
        }

        void add_key(const Key &key) {
            keys->insert({key, timestamp++});
        }

        void remove_key(const Key &key) {
            keys->erase(key);
        }

    private:
        /// pointer to a mapping from keys to insertion timestamps
        std::unordered_map<Key, std::size_t> *keys = nullptr;
        /// the next valid insertion timestamp
        size_t timestamp = 1;
    };


    template<
            class Key,
            class T,
            class Compare = fifo_map_compare<Key>,
            class Allocator = std::allocator<std::pair<const Key, T>>
    >
    class fifo_map // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions,-warnings-as-errors)
    {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<const Key, T>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using key_compare = Compare;
        using allocator_type = Allocator;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

        using internal_map_type = std::map<Key, T, Compare, Allocator>;

        using iterator = typename internal_map_type::iterator;
        using const_iterator = typename internal_map_type::const_iterator;
        using reverse_iterator = typename internal_map_type::reverse_iterator;
        using const_reverse_iterator = typename internal_map_type::const_reverse_iterator;

    public:
        /// default constructor
        fifo_map() : m_keys(), m_compare(&m_keys), m_map(m_compare) {}

        /// copy constructor
        fifo_map(const fifo_map &f) : m_keys(f.m_keys), m_compare(&m_keys),
                                      m_map(f.m_map.begin(), f.m_map.end(), m_compare) {}

        /// constructor for a range of elements
        template<class InputIterator>
        fifo_map(InputIterator first, InputIterator last)
                : m_keys(), m_compare(&m_keys), m_map(m_compare) {
            for (auto it = first; it != last; ++it) {
                insert(*it);
            }
        }

        /// constructor for a list of elements
        fifo_map(std::initializer_list<value_type> init) : fifo_map() {
            for (auto x: init) {
                insert(x);
            }
        }


        /*
         * Element access
         */

        /// access specified element with bounds checking
        T &at(const Key &key) {
            return m_map.at(key);
        }

        /// access specified element with bounds checking
        const T &at(const Key &key) const {
            return m_map.at(key);
        }

        /// access specified element
        T &operator[](const Key &key) {
            m_compare.add_key(key);
            return m_map[key];
        }

        /// access specified element
        T &operator[](Key &&key) {
            m_compare.add_key(key);
            return m_map[key];
        }


        /*
         * Iterators
         */

        /// returns an iterator to the beginning
        iterator begin() noexcept {
            return m_map.begin();
        }

        /// returns an iterator to the end
        iterator end() noexcept {
            return m_map.end();
        }

        /// returns an iterator to the beginning
        const_iterator begin() const noexcept {
            return m_map.begin();
        }

        /// returns an iterator to the end
        const_iterator end() const noexcept {
            return m_map.end();
        }

        /// returns an iterator to the beginning
        const_iterator cbegin() const noexcept {
            return m_map.cbegin();
        }

        /// returns an iterator to the end
        const_iterator cend() const noexcept {
            return m_map.cend();
        }

        /// returns a reverse iterator to the beginning
        reverse_iterator rbegin() noexcept {
            return m_map.rbegin();
        }

        /// returns a reverse iterator to the end
        reverse_iterator rend() noexcept {
            return m_map.rend();
        }

        /// returns a reverse iterator to the beginning
        const_reverse_iterator rbegin() const noexcept {
            return m_map.rbegin();
        }

        /// returns a reverse iterator to the end
        const_reverse_iterator rend() const noexcept {
            return m_map.rend();
        }

        /// returns a reverse iterator to the beginning
        const_reverse_iterator crbegin() const noexcept {
            return m_map.crbegin();
        }

        /// returns a reverse iterator to the end
        const_reverse_iterator crend() const noexcept {
            return m_map.crend();
        }


        /*
         * Capacity
         */

        /// checks whether the container is empty
        bool empty() const noexcept {
            return m_map.empty();
        }

        /// returns the number of elements
        size_type size() const noexcept {
            return m_map.size();
        }

        /// returns the maximum possible number of elements
        size_type max_size() const noexcept {
            return m_map.max_size();
        }


        /*
         * Modifiers
         */

        /// clears the contents
        void clear() noexcept {
            m_map.clear();
            m_keys.clear();
        }

        /// insert value
        std::pair<iterator, bool> insert(const value_type &value) {
            m_compare.add_key(value.first);
            return m_map.insert(value);
        }

        /// insert value
        template<class P>
        std::pair<iterator, bool> insert(P &&value) // NOLINT(cppcoreguidelines-missing-std-forward)
        {
            m_compare.add_key(value.first);
            return m_map.insert(value);
        }

        /// insert value with hint
        iterator insert(const_iterator hint, const value_type &value) {
            m_compare.add_key(value.first);
            return m_map.insert(hint, value);
        }

        /// insert value with hint
        iterator insert(const_iterator hint, value_type &&value) {
            m_compare.add_key(value.first);
            return m_map.insert(hint, value);
        }

        /// insert value range
        template<class InputIt>
        void insert(InputIt first, InputIt last) {
            for (const_iterator it = first; it != last; ++it) {
                m_compare.add_key(it->first);
            }

            m_map.insert(first, last);
        }

        /// insert value list
        void insert(std::initializer_list<value_type> ilist) {
            for (auto value: ilist) {
                m_compare.add_key(value.first);
            }

            m_map.insert(ilist);
        }

        /// constructs element in-place
        template<class... Args>
        std::pair<iterator, bool> emplace(Args &&... args) {
            typename fifo_map::value_type value(std::forward<Args>(args)...);
            m_compare.add_key(value.first);
            return m_map.emplace(std::move(value));
        }

        /// constructs element in-place with hint
        template<class... Args>
        iterator emplace_hint(const_iterator hint, Args &&... args) {
            typename fifo_map::value_type value(std::forward<Args>(args)...);
            m_compare.add_key(value.first);
            return m_map.emplace_hint(hint, std::move(value));
        }

        /// remove element at position
        iterator erase(const_iterator pos) {
            m_compare.remove_key(pos->first);
            return m_map.erase(pos);
        }

        /// remove elements in range
        iterator erase(const_iterator first, const_iterator last) {
            for (const_iterator it = first; it != last; ++it) {
                m_compare.remove_key(it->first);
            }

            return m_map.erase(first, last);
        }

        /// remove elements with key
        size_type erase(const key_type &key) {
            size_type res = m_map.erase(key);

            if (res > 0) {
                m_compare.remove_key(key);
            }

            return res;
        }

        /// swaps the contents
        void swap(fifo_map &other) // NOLINT(cppcoreguidelines-noexcept-swap,performance-noexcept-swap)
        {
            std::swap(m_map, other.m_map);
            std::swap(m_compare, other.m_compare);
            std::swap(m_keys, other.m_keys);
        }


        /*
         * Lookup
         */

        /// returns the number of elements matching specific key
        size_type count(const Key &key) const {
            return m_map.count(key);
        }

        /// finds element with specific key
        iterator find(const Key &key) {
            return m_map.find(key);
        }

        /// finds element with specific key
        const_iterator find(const Key &key) const {
            return m_map.find(key);
        }

        /// returns range of elements matching a specific key
        std::pair<iterator, iterator> equal_range(const Key &key) {
            return m_map.equal_range(key);
        }

        /// returns range of elements matching a specific key
        std::pair<const_iterator, const_iterator> equal_range(const Key &key) const {
            return m_map.equal_range(key);
        }

        /// returns an iterator to the first element not less than the given key
        iterator lower_bound(const Key &key) {
            return m_map.lower_bound(key);
        }

        /// returns an iterator to the first element not less than the given key
        const_iterator lower_bound(const Key &key) const {
            return m_map.lower_bound(key);
        }

        /// returns an iterator to the first element greater than the given key
        iterator upper_bound(const Key &key) {
            return m_map.upper_bound(key);
        }

        /// returns an iterator to the first element greater than the given key
        const_iterator upper_bound(const Key &key) const {
            return m_map.upper_bound(key);
        }


        /*
         * Observers
         */

        /// returns the function that compares keys
        key_compare key_comp() const {
            return m_compare;
        }


        /*
         * Non-member functions
         */

        friend bool operator==(const fifo_map &lhs, const fifo_map &rhs) {
            return lhs.m_map == rhs.m_map;
        }

        friend bool operator!=(const fifo_map &lhs, const fifo_map &rhs) {
            return lhs.m_map != rhs.m_map;
        }

        friend bool operator<(const fifo_map &lhs, const fifo_map &rhs) {
            return lhs.m_map < rhs.m_map;
        }

        friend bool operator<=(const fifo_map &lhs, const fifo_map &rhs) {
            return lhs.m_map <= rhs.m_map;
        }

        friend bool operator>(const fifo_map &lhs, const fifo_map &rhs) {
            return lhs.m_map > rhs.m_map;
        }

        friend bool operator>=(const fifo_map &lhs, const fifo_map &rhs) {
            return lhs.m_map >= rhs.m_map;
        }

    private:
        /// the keys
        std::unordered_map<Key, std::size_t> m_keys;
        /// the comparison object
        Compare m_compare;
        /// the internal data structure
        internal_map_type m_map;
    };

} // namespace collie

// specialization of std::swap
namespace std // NOLINT(cert-dcl58-cpp,-warnings-as-errors)
{
    template<class Key, class T, class Compare, class Allocator>
    inline void
    swap(collie::fifo_map <Key, T, Compare, Allocator> &m1, // NOLINT(cert-dcl58-cpp,cppcoreguidelines-noexcept-swap,performance-noexcept-swap)
         collie::fifo_map <Key, T, Compare, Allocator> &m2) {
        m1.swap(m2);
    }
} // namespace std

#endif  // COLLIE_CONTAINER_FIFO_MAP_H_
