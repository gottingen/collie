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

namespace collie::tf {

// ----------------------------------------------------------------------------

    class TopologyBase {

    };

// class: Topology
    class Topology {

        friend class Executor;

        friend class Runtime;

        friend class Node;

        template<typename T>
        friend
        class Future;

        constexpr static int CLEAN = 0;
        constexpr static int CANCELLED = 1;
        constexpr static int EXCEPTION = 2;

    public:

        template<typename P, typename C>
        Topology(Taskflow &, P &&, C &&);

        bool cancelled() const;

    private:

        Taskflow &_taskflow;

        std::promise<void> _promise;

        InlinedVector<Node *> _sources;

        std::function<bool()> _pred;
        std::function<void()> _call;

        std::atomic<size_t> _join_counter{0};
        std::atomic<int> _state{CLEAN};

        std::exception_ptr _exception_ptr{nullptr};

        void _carry_out_promise();
    };

// Constructor
    template<typename P, typename C>
    Topology::Topology(Taskflow &tf, P &&p, C &&c):
            _taskflow(tf),
            _pred{std::forward<P>(p)},
            _call{std::forward<C>(c)} {
    }

// Procedure
    inline void Topology::_carry_out_promise() {
        if (_exception_ptr) {
            auto e = _exception_ptr;
            _exception_ptr = nullptr;
            _promise.set_exception(e);
        } else {
            _promise.set_value();
        }
    }

// Function: cancelled
    inline bool Topology::cancelled() const {
        return _state.load(std::memory_order_relaxed) & CANCELLED;
    }

}  // end of namespace collie::tf. ----------------------------------------------------
