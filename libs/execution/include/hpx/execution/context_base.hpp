//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_EXECUTION_CONTEXT_BASE_HPP
#define HPX_EXECUTION_CONTEXT_BASE_HPP

#include <hpx/execution/resource_base.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <cstdint>

namespace hpx { namespace execution {
    struct context_base
    {
        virtual ~context_base() {}

        virtual resource_base const& resource() const = 0;

    };
}}    // namespace hpx::execution

#endif
