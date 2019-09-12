//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_EXECUTION_AGENT_BASE_HPP
#define HPX_EXECUTION_AGENT_BASE_HPP

#include <hpx/execution/context_base.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <cstdint>
#include <string>

namespace hpx { namespace execution {
    struct agent_base
    {
        virtual ~agent_base() {}

        virtual std::string description() const = 0;

        virtual context_base const& context() const = 0;

        virtual void yield(const char* desc) = 0;
        virtual void yield_k(std::size_t k, const char* desc) = 0;
        virtual void suspend(const char* desc) = 0;
        virtual void resume(const char* desc) = 0;
        virtual void abort(const char* desc) = 0;
        virtual void sleep_for(hpx::util::steady_duration const& sleep_duration,
            const char* desc) = 0;
        virtual void sleep_until(hpx::util::steady_time_point const& sleep_time,
            const char* desc) = 0;
    };
}}    // namespace hpx::execution

#endif
