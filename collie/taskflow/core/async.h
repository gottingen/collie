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

#include <collie/taskflow/core/executor.h>

// https://hackmd.io/@sysprog/concurrency-atomics

namespace collie::tf {

    // ----------------------------------------------------------------------------
    // Async
    // ----------------------------------------------------------------------------

    // Function: async
    template<typename P, typename F>
    auto Executor::async(P &&params, F &&f) {

        _increment_topology();

        using R = std::invoke_result_t<std::decay_t<F>>;

        std::packaged_task<R()> p(std::forward<F>(f));
        auto fu{p.get_future()};

        auto node = node_pool.animate(
                std::forward<P>(params), nullptr, nullptr, 0,
                // handle
                std::in_place_type_t<Node::Async>{},
                [p = make_moc(std::move(p))]() mutable { p.object(); }
        );

        _schedule_async_task(node);

        return fu;
    }

    // Function: async
    template<typename F>
    auto Executor::async(F &&f) {
        return async(DefaultTaskParams{}, std::forward<F>(f));
    }

    // ----------------------------------------------------------------------------
    // Silent Async
    // ----------------------------------------------------------------------------

    // Function: silent_async
    template<typename P, typename F>
    void Executor::silent_async(P &&params, F &&f) {

        _increment_topology();

        auto node = node_pool.animate(
                std::forward<P>(params), nullptr, nullptr, 0,
                // handle
                std::in_place_type_t<Node::Async>{}, std::forward<F>(f)
        );

        _schedule_async_task(node);
    }

    // Function: silent_async
    template<typename F>
    void Executor::silent_async(F &&f) {
        silent_async(DefaultTaskParams{}, std::forward<F>(f));
    }

    // ----------------------------------------------------------------------------
    // Async Helper Methods
    // ----------------------------------------------------------------------------

    // Procedure: _schedule_async_task
    inline void Executor::_schedule_async_task(Node *node) {
        if (auto w = _this_worker(); w) {
            _schedule(*w, node);
        } else {
            _schedule(node);
        }
    }

    // Procedure: _tear_down_async
    inline void Executor::_tear_down_async(Node *node) {
        // from runtime
        if (node->_parent) {
            node->_parent->_join_counter.fetch_sub(1, std::memory_order_release);
        }
            // from executor
        else {
            _decrement_topology();
        }
        node_pool.recycle(node);
    }

    // ----------------------------------------------------------------------------
    // Silent Dependent Async
    // ----------------------------------------------------------------------------

    // Function: silent_dependent_async
    template<typename F, typename... Tasks,
            std::enable_if_t<all_same_v<AsyncTask, std::decay_t<Tasks>...>, void> *
    >
    collie::tf::AsyncTask Executor::silent_dependent_async(F &&func, Tasks &&... tasks) {
        return silent_dependent_async(
                DefaultTaskParams{}, std::forward<F>(func), std::forward<Tasks>(tasks)...
        );
    }

    // Function: silent_dependent_async
    template<typename P, typename F, typename... Tasks,
            std::enable_if_t<is_task_params_v<P> && all_same_v<AsyncTask, std::decay_t<Tasks>...>, void> *
    >
    collie::tf::AsyncTask Executor::silent_dependent_async(
            P &&params, F &&func, Tasks &&... tasks
    ) {

        _increment_topology();

        size_t num_dependents = sizeof...(Tasks);

        // create a task before scheduling the node to retain a shared ownership first
        AsyncTask task(node_pool.animate(
                std::forward<P>(params), nullptr, nullptr, num_dependents,
                std::in_place_type_t<Node::DependentAsync>{}, std::forward<F>(func)
        ));

        if constexpr (sizeof...(Tasks) > 0) {
            (_process_async_dependent(task._node, tasks, num_dependents), ...);
        }

        if (num_dependents == 0) {
            _schedule_async_task(task._node);
        }

        return task;
    }

    // Function: silent_dependent_async
    template<typename F, typename I,
            std::enable_if_t<!std::is_same_v<std::decay_t<I>, AsyncTask>, void> *
    >
    collie::tf::AsyncTask Executor::silent_dependent_async(F &&func, I first, I last) {
        return silent_dependent_async(DefaultTaskParams{}, std::forward<F>(func), first, last);
    }

    // Function: silent_dependent_async
    template<typename P, typename F, typename I,
            std::enable_if_t<is_task_params_v<P> && !std::is_same_v<std::decay_t<I>, AsyncTask>, void> *
    >
    collie::tf::AsyncTask Executor::silent_dependent_async(
            P &&params, F &&func, I first, I last
    ) {

        _increment_topology();

        size_t num_dependents = std::distance(first, last);

        AsyncTask task(node_pool.animate(
                std::forward<P>(params), nullptr, nullptr, num_dependents,
                std::in_place_type_t<Node::DependentAsync>{}, std::forward<F>(func)
        ));

        for (; first != last; first++) {
            _process_async_dependent(task._node, *first, num_dependents);
        }

        if (num_dependents == 0) {
            _schedule_async_task(task._node);
        }

        return task;
    }

    // ----------------------------------------------------------------------------
    // Dependent Async
    // ----------------------------------------------------------------------------

    // Function: dependent_async
    template<typename F, typename... Tasks,
            std::enable_if_t<all_same_v<AsyncTask, std::decay_t<Tasks>...>, void> *
    >
    auto Executor::dependent_async(F &&func, Tasks &&... tasks) {
        return dependent_async(DefaultTaskParams{}, std::forward<F>(func), std::forward<Tasks>(tasks)...);
    }

    // Function: dependent_async
    template<typename P, typename F, typename... Tasks,
            std::enable_if_t<is_task_params_v<P> && all_same_v<AsyncTask, std::decay_t<Tasks>...>, void> *
    >
    auto Executor::dependent_async(P &&params, F &&func, Tasks &&... tasks) {

        _increment_topology();

        using R = std::invoke_result_t<std::decay_t<F>>;

        std::packaged_task<R()> p(std::forward<F>(func));
        auto fu{p.get_future()};

        size_t num_dependents = sizeof...(tasks);

        AsyncTask task(node_pool.animate(
                std::forward<P>(params), nullptr, nullptr, num_dependents,
                std::in_place_type_t<Node::DependentAsync>{},
                [p = make_moc(std::move(p))]() mutable { p.object(); }
        ));

        if constexpr (sizeof...(Tasks) > 0) {
            (_process_async_dependent(task._node, tasks, num_dependents), ...);
        }

        if (num_dependents == 0) {
            _schedule_async_task(task._node);
        }

        return std::make_pair(std::move(task), std::move(fu));
    }

    // Function: dependent_async
    template<typename F, typename I,
            std::enable_if_t<!std::is_same_v<std::decay_t<I>, AsyncTask>, void> *
    >
    auto Executor::dependent_async(F &&func, I first, I last) {
        return dependent_async(DefaultTaskParams{}, std::forward<F>(func), first, last);
    }

    // Function: dependent_async
    template<typename P, typename F, typename I,
            std::enable_if_t<is_task_params_v<P> && !std::is_same_v<std::decay_t<I>, AsyncTask>, void> *
    >
    auto Executor::dependent_async(P &&params, F &&func, I first, I last) {

        _increment_topology();

        using R = std::invoke_result_t<std::decay_t<F>>;

        std::packaged_task<R()> p(std::forward<F>(func));
        auto fu{p.get_future()};

        size_t num_dependents = std::distance(first, last);

        AsyncTask task(node_pool.animate(
                std::forward<P>(params), nullptr, nullptr, num_dependents,
                std::in_place_type_t<Node::DependentAsync>{},
                [p = make_moc(std::move(p))]() mutable { p.object(); }
        ));

        for (; first != last; first++) {
            _process_async_dependent(task._node, *first, num_dependents);
        }

        if (num_dependents == 0) {
            _schedule_async_task(task._node);
        }

        return std::make_pair(std::move(task), std::move(fu));
    }

    // ----------------------------------------------------------------------------
    // Dependent Async Helper Functions
    // ----------------------------------------------------------------------------

    // Procedure: _process_async_dependent
    inline void Executor::_process_async_dependent(
            Node *node, collie::tf::AsyncTask &task, size_t &num_dependents
    ) {

        auto &state = std::get_if<Node::DependentAsync>(&(task._node->_handle))->state;

        add_successor:

        auto target = Node::AsyncState::UNFINISHED;

        // acquires the lock
        if (state.compare_exchange_weak(target, Node::AsyncState::LOCKED,
                                        std::memory_order_acq_rel,
                                        std::memory_order_acquire)) {
            task._node->_successors.push_back(node);
            state.store(Node::AsyncState::UNFINISHED, std::memory_order_release);
        }
            // dep's state is FINISHED, which means dep finished its callable already
            // thus decrement the node's join counter by 1
        else if (target == Node::AsyncState::FINISHED) {
            num_dependents = node->_join_counter.fetch_sub(1, std::memory_order_acq_rel) - 1;
        }
            // another worker adding its async task to the same successors of this node
        else {
            goto add_successor;
        }
    }


    // Procedure: _tear_down_dependent_async
    inline void Executor::_tear_down_dependent_async(Worker &worker, Node *node) {

        auto handle = std::get_if<Node::DependentAsync>(&(node->_handle));

        // this async task comes from Executor
        auto target = Node::AsyncState::UNFINISHED;

        while (!handle->state.compare_exchange_weak(target, Node::AsyncState::FINISHED,
                                                    std::memory_order_acq_rel,
                                                    std::memory_order_relaxed)) {
            target = Node::AsyncState::UNFINISHED;
        }

        // spaw successors whenever their dependencies are resolved
        worker._cache = nullptr;
        for (size_t i = 0; i < node->_successors.size(); ++i) {
            if (auto s = node->_successors[i];
                    s->_join_counter.fetch_sub(1, std::memory_order_acq_rel) == 1
                    ) {
                if (worker._cache) {
                    _schedule(worker, worker._cache);
                }
                worker._cache = s;
            }
        }

        // now the executor no longer needs to retain ownership
        if (handle->use_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            node_pool.recycle(node);
        }

        _decrement_topology();
    }


}  // end of namespace collie::tf -----------------------------------------------------

