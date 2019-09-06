//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/assertion.hpp>
#include <hpx/execution/execution_context.hpp>
#ifdef HPX_HAVE_VERIFY_LOCKS
#include <hpx/execution/register_locks.hpp>
#endif
#include <hpx/execution/this_thread.hpp>
#include <hpx/format.hpp>

#include <iostream>

namespace hpx { namespace execution {
    void execution_context::yield(const char* desc)
    {
        HPX_ASSERT(*this == hpx::execution::this_thread::execution_context());
        // verify that there are no more registered locks for this OS-thread
#ifdef HPX_HAVE_VERIFY_LOCKS
        util::verify_no_locks();
#endif
        impl_->yield(desc);
    }

    void execution_context::yield_k(std::size_t k, const char* desc)
    {
        HPX_ASSERT(*this == hpx::execution::this_thread::execution_context());
        // verify that there are no more registered locks for this OS-thread
#ifdef HPX_HAVE_VERIFY_LOCKS
        util::verify_no_locks();
#endif
        impl_->yield(desc);
    }

    void execution_context::suspend(const char* desc)
    {
        HPX_ASSERT(*this == hpx::execution::this_thread::execution_context());
        // verify that there are no more registered locks for this OS-thread
#ifdef HPX_HAVE_VERIFY_LOCKS
        util::verify_no_locks();
#endif
        impl_->suspend(desc);
    }

    void execution_context::resume(const char* desc)
    {
        HPX_ASSERT(*this != hpx::execution::this_thread::execution_context());
        impl_->resume(desc);
    }

    void execution_context::abort(const char* desc)
    {
        HPX_ASSERT(*this != hpx::execution::this_thread::execution_context());
        impl_->abort(desc);
    }

    std::ostream& operator<<(std::ostream& os, execution_context const& ctx)
    {
        hpx::util::format_to(os, "execution_context{{{}}}", ctx.impl_);
        return os;
    }
}}    // namespace hpx::execution
