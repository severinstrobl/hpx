//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/assertion.hpp>
#include <hpx/execution/agent.hpp>
#ifdef HPX_HAVE_VERIFY_LOCKS
#include <hpx/execution/register_locks.hpp>
#endif
#include <hpx/execution/this_thread.hpp>
#include <hpx/format.hpp>

#include <iostream>

namespace hpx { namespace execution {
    void agent::yield(const char* desc)
    {
        HPX_ASSERT(*this == hpx::execution::this_thread::agent());
        // verify that there are no more registered locks for this OS-thread
#ifdef HPX_HAVE_VERIFY_LOCKS
        util::verify_no_locks();
#endif
        impl_->yield(desc);
    }

    void agent::yield_k(std::size_t k, const char* desc)
    {
        HPX_ASSERT(*this == hpx::execution::this_thread::agent());
        // verify that there are no more registered locks for this OS-thread
#ifdef HPX_HAVE_VERIFY_LOCKS
        util::verify_no_locks();
#endif
        impl_->yield(desc);
    }

    void agent::suspend(const char* desc)
    {
        HPX_ASSERT(*this == hpx::execution::this_thread::agent());
        // verify that there are no more registered locks for this OS-thread
#ifdef HPX_HAVE_VERIFY_LOCKS
        util::verify_no_locks();
#endif
        impl_->suspend(desc);
    }

    void agent::resume(const char* desc)
    {
        HPX_ASSERT(*this != hpx::execution::this_thread::agent());
        impl_->resume(desc);
    }

    void agent::abort(const char* desc)
    {
        HPX_ASSERT(*this != hpx::execution::this_thread::agent());
        impl_->abort(desc);
    }

    std::ostream& operator<<(std::ostream& os, agent const& a)
    {
        hpx::util::format_to(os, "agent{{{}}}", a.impl_->description());
        return os;
    }
}}    // namespace hpx::execution
