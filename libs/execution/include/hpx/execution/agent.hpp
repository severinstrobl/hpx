//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_EXECUTION_EXECUTION_CONTEXT_HPP
#define HPX_EXECUTION_EXECUTION_CONTEXT_HPP

#include <hpx/config.hpp>
#include <hpx/execution/agent_base.hpp>

#include <iosfwd>

namespace hpx { namespace execution {
    class HPX_EXPORT agent
    {
    public:
        constexpr agent() noexcept
          : impl_(nullptr)
        {
        }
        constexpr agent(agent_base* impl) noexcept
          : impl_(impl)
        {
        }
        constexpr agent(
            agent const&) noexcept = default;
        constexpr agent& operator=(
            agent const&) noexcept = default;

        constexpr agent(agent&&) noexcept = default;
        constexpr agent& operator=(
            agent&&) noexcept = default;

        constexpr explicit operator bool() noexcept
        {
            return impl_ != nullptr;
        }

        void yield(const char* desc = "hpx::execution::agent::yield");
        void yield_k(std::size_t k,
            const char* desc = "hpx::execution::agent::yield_k");
        void suspend(const char* desc = "hpx::execution::agent::suspend");
        void resume(const char* desc = "hpx::execution::agent::resume");
        void abort(const char* desc = "hpx::execution::agent::abort");

        template <class Rep, class Period>
        void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration,
            const char* desc = "hpx::execution::agent::sleep_for")
        {
            impl_->sleep_for(sleep_duration, desc);
        }

        template <class Clock, class Duration>
        void sleep_until(
            const std::chrono::time_point<Clock, Duration>& sleep_time,
            const char* desc = "hpx::execution::agent::sleep_until")
        {
            impl_->sleep_until(sleep_time, desc);
        }

        // TODO:
        // affinity
        // thread_num
        // executor

    private:
        agent_base* impl_;

        friend constexpr bool operator==(
            agent const& lhs, agent const& rhs)
        {
            return lhs.impl_ == rhs.impl_;
        }

        friend constexpr bool operator!=(
            agent const& lhs, agent const& rhs)
        {
            return lhs.impl_ != rhs.impl_;
        }

        friend std::ostream& operator<<(std::ostream&, agent const&);
    };
}}    // namespace hpx::execution

#endif
