//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_EXECUTION_EXECUTION_CONTEXT_HPP
#define HPX_EXECUTION_EXECUTION_CONTEXT_HPP

#include <hpx/config.hpp>
#include <hpx/execution/execution_context_base.hpp>

#include <iosfwd>

namespace hpx { namespace execution {
    class HPX_EXPORT execution_context
    {
    public:
        constexpr execution_context() noexcept
          : impl_(nullptr)
        {
        }
        constexpr execution_context(execution_context_base* impl) noexcept
          : impl_(impl)
        {
        }
        constexpr execution_context(
            execution_context const&) noexcept = default;
        constexpr execution_context& operator=(
            execution_context const&) noexcept = default;

        constexpr execution_context(execution_context&&) noexcept = default;
        constexpr execution_context& operator=(
            execution_context&&) noexcept = default;

        constexpr explicit operator bool() noexcept
        {
            return impl_ != nullptr;
        }

        void yield(
            const char* desc = "hpx::execution::execution_context::yield");
        void yield_k(std::size_t k,
            const char* desc = "hpx::execution::execution_context::yield_k");
        void suspend(
            const char* desc = "hpx::execution::execution_context::suspend");
        void resume(
            const char* desc = "hpx::execution::execution_context::resume");
        void abort(
            const char* desc = "hpx::execution::execution_context::abort");

        template <class Rep, class Period>
        void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration,
            const char* desc = "hpx::execution::execution_context::sleep_for")
        {
            impl_->sleep_for(sleep_duration, desc);
        }

        template <class Clock, class Duration>
        void sleep_until(
            const std::chrono::time_point<Clock, Duration>& sleep_time,
            const char* desc = "hpx::execution::execution_context::sleep_until")
        {
            impl_->sleep_until(sleep_time, desc);
        }

        // TODO:
        // affinity
        // thread_num
        // executor

    private:
        execution_context_base* impl_;

        friend constexpr bool operator==(
            execution_context const& lhs, execution_context const& rhs)
        {
            return lhs.impl_ == rhs.impl_;
        }

        friend constexpr bool operator!=(
            execution_context const& lhs, execution_context const& rhs)
        {
            return lhs.impl_ != rhs.impl_;
        }

        friend std::ostream& operator<<(
            std::ostream&, execution_context const&);
    };
}}    // namespace hpx::execution

#endif
