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
#include <initializer_list>
#include <iterator>
#include <cstdarg>
#include <cstdio>

namespace turbo::testing {


    template<typename InputIterator1, typename InputIterator2>
    bool check_sequence_eq(InputIterator1 firstActual, InputIterator1 lastActual, InputIterator2 firstExpected,
                           InputIterator2 lastExpected, const char *pName) {
        size_t numMatching = 0;

        while ((firstActual != lastActual) && (firstExpected != lastExpected) && (*firstActual == *firstExpected)) {
            ++firstActual;
            ++firstExpected;
            ++numMatching;
        }

        if (firstActual == lastActual && firstExpected == lastExpected) {
            return true;
        } else if (firstActual != lastActual && firstExpected == lastExpected) {
            size_t numActual = numMatching, numExpected = numMatching;
            for (; firstActual != lastActual; ++firstActual)
                ++numActual;
            if (pName)
                printf("[%s] Too many elements: expected %u, found %u\n", pName, numExpected, numActual);
            else
                printf("Too many elements: expected %u, found %u\n", numExpected, numActual);
            return false;
        } else if (firstActual == lastActual && firstExpected != lastExpected) {
            size_t numActual = numMatching, numExpected = numMatching;
            for (; firstExpected != lastExpected; ++firstExpected)
                ++numExpected;
            if (pName)
                printf("[%s] Too few elements: expected %u, found %u\n", pName, numExpected, numActual);
            else
                printf("Too few elements: expected %u, found %u\n", numExpected, numActual);
            return false;
        } else // if (firstActual != lastActual && firstExpected != lastExpected)
        {
            if (pName)
                printf("[%s] Mismatch at index %u\n", pName, numMatching);
            else
                printf("Mismatch at index %u\n", numMatching);
            return false;
        }
    }

    template<typename InputIterator, typename T = typename InputIterator::value_type>
    bool check_sequence_eq(InputIterator firstActual, InputIterator lastActual, std::initializer_list<T> initList,
                           const char *pName) {
        return check_sequence_eq(firstActual, lastActual, initList.begin(), initList.end(), pName);
    }

    template<typename Container, typename T = typename Container::value_type>
    bool check_sequence_eq(const Container &container, std::initializer_list<T> initList, const char *pName) {
        return check_sequence_eq(container.begin(), container.end(), initList.begin(), initList.end(), pName);
    }


    /// check_sequence_eq
    ///
    /// Allows the user to specify that a container has a given set of values.
    ///
    /// Example usage:
    ///    vector<int> v;
    ///    v.push_back(1); v.push_back(3); v.push_back(5);
    ///    check_sequence_eq(v.begin(), v.end(), int(), "v.push_back", 1, 3, 5, -1);
    ///
    /// Note: The StackValue template argument is a hint to the compiler about what type
    ///       the passed vararg sequence is.
    ///
    template<typename InputIterator, typename StackValue>
    bool check_sequence_eq(InputIterator first, InputIterator last, StackValue /*unused*/, const char *pName, ...) {
        typedef typename std::iterator_traits<InputIterator>::value_type value_type;

        int argIndex = 0;
        int seqIndex = 0;
        bool bReturnValue = true;
        StackValue next;

        va_list args;
        va_start(args, pName);

        for (; first != last; ++first, ++argIndex, ++seqIndex) {
            next = va_arg(args, StackValue);

            if ((next == StackValue(-1)) || !(value_type(next) == *first)) {
                if (pName)
                    printf("[%s] Mismatch at index %d\n", pName, argIndex);
                else
                    printf("Mismatch at index %d\n", argIndex);
                bReturnValue = false;
            }
        }

        for (; first != last; ++first)
            ++seqIndex;

        if (bReturnValue) {
            next = va_arg(args, StackValue);

            if (!(next == StackValue(-1))) {
                do {
                    ++argIndex;
                    next = va_arg(args, StackValue);
                } while (!(next == StackValue(-1)));

                if (pName)
                    printf("[%s] Too many elements: expected %d, found %d\n", pName, argIndex, seqIndex);
                else
                    printf("Too many elements: expected %d, found %d\n", argIndex, seqIndex);
                bReturnValue = false;
            }
        }

        va_end(args);

        return bReturnValue;
    }

}  // namespace turbo::testing
