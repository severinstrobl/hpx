//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_THREADS_EXECUTION_CONTEXT_HPP
#define HPX_THREADS_EXECUTION_CONTEXT_HPP

#include <hpx/config.hpp>

#include <hpx/execution/execution_context_base.hpp>
#include <hpx/runtime/threads/coroutines/detail/coroutine_impl.hpp>
#include <hpx/runtime/threads/coroutines/detail/coroutine_self.hpp>
#include <hpx/runtime/threads/thread_enums.hpp>
#include <hpx/runtime/threads/thread_id_type.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <cstdint>

namespace hpx { namespace threads {

    struct HPX_EXPORT execution_context : hpx::execution::execution_context_base
    {
        explicit execution_context(
            coroutines::detail::coroutine_impl* coroutine) noexcept;

        void yield(const char* desc);
        void yield_k(std::size_t k, const char* desc);
        void suspend(const char* desc);
        void resume(const char* desc);
        void abort(const char* desc);
        void sleep_for(
            hpx::util::steady_duration const& sleep_duration, const char* desc);
        void sleep_until(
            hpx::util::steady_time_point const& sleep_time, const char* desc);

    private:
        coroutines::detail::coroutine_self self_;

        hpx::threads::thread_state_ex_enum do_yield(const char* desc,
                threads::thread_state_enum state);

        void do_resume(const char* desc, hpx::threads::thread_state_ex_enum statex);

    };
}}    // namespace hpx::threads

#endif
