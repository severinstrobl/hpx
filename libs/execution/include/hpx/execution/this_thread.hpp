//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_EXECUTION_THIS_THREAD_HPP
#define HPX_EXECUTION_THIS_THREAD_HPP

#include <hpx/execution/execution_context.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <cstdint>

namespace hpx { namespace execution { namespace this_thread {
    namespace detail {
        struct execution_context_storage;
        execution_context_storage* get_execution_context_storage();
    }    // namespace detail

    struct reset_execution_context
    {
        reset_execution_context(
            detail::execution_context_storage*, execution_context_base& impl);
        reset_execution_context(execution_context_base& impl);
        ~reset_execution_context();

        detail::execution_context_storage* storage_;
        execution_context_base* old_;
    };

    hpx::execution::execution_context execution_context();

    void yield(const char* desc = "hpx::execution::this_thread::yield");
    void yield_k(std::size_t k,
        const char* desc = "hpx::execution::this_thread::yield_k");
    void suspend(const char* desc = "hpx::execution::this_thread::suspend");

    template <typename Rep, typename Period>
    void sleep_for(std::chrono::duration<Rep, Period> const& sleep_duration,
        const char* desc = "hpx::execution::this_thread::sleep_for")
    {
        execution_context().sleep_for(sleep_duration, desc);
    }

    template <class Clock, class Duration>
    void sleep_until(const std::chrono::time_point<Clock, Duration>& sleep_time,
        const char* desc = "hpx::execution::this_thread::sleep_for")
    {
        execution_context().sleep_until(sleep_time, desc);
    }
}}}    // namespace hpx::execution::this_thread

#endif
