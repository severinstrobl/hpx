//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_EXECUTION_THIS_THREAD_HPP
#define HPX_EXECUTION_THIS_THREAD_HPP

#include <hpx/config.hpp>
#include <hpx/execution/agent_ref.hpp>
#include <hpx/execution/agent_base.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <chrono>
#include <cstdint>

namespace hpx { namespace execution { namespace this_thread {
    namespace detail {
        struct agent_storage;
        HPX_EXPORT agent_storage* get_agent_storage();
    }    // namespace detail

    struct HPX_EXPORT reset_agent
    {
        reset_agent(
            detail::agent_storage*, agent_base& impl);
        reset_agent(agent_base& impl);
        ~reset_agent();

        detail::agent_storage* storage_;
        agent_base* old_;
    };

    HPX_EXPORT hpx::execution::agent_ref agent();

    HPX_EXPORT void yield(const char* desc = "hpx::execution::this_thread::yield");
    HPX_EXPORT void yield_k(std::size_t k,
        const char* desc = "hpx::execution::this_thread::yield_k");
    HPX_EXPORT void suspend(const char* desc = "hpx::execution::this_thread::suspend");

    template <typename Rep, typename Period>
    void sleep_for(std::chrono::duration<Rep, Period> const& sleep_duration,
        const char* desc = "hpx::execution::this_thread::sleep_for")
    {
        agent().sleep_for(sleep_duration, desc);
    }

    template <class Clock, class Duration>
    void sleep_until(const std::chrono::time_point<Clock, Duration>& sleep_time,
        const char* desc = "hpx::execution::this_thread::sleep_for")
    {
        agent().sleep_until(sleep_time, desc);
    }
}}}    // namespace hpx::execution::this_thread

#endif
