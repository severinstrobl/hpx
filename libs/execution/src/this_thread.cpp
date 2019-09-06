//  Copyright (c) 2013-2019 Thomas Heller
//  Copyright (c) 2008 Peter Dimov
//  Copyright (c) 2018 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/assertion.hpp>
#include <hpx/errors/throw_exception.hpp>
#include <hpx/execution/this_thread.hpp>

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <sstream>
#include <thread>

#if defined(HPX_WINDOWS)
#include <windows.h>
#else
#ifndef _AIX
#include <sched.h>
#else
// AIX's sched.h defines ::var which sometimes conflicts with Lambda's var
extern "C" int sched_yield(void);
#endif
#include <time.h>
#endif

namespace hpx { namespace execution {
    namespace {
        struct default_execution_context : execution_context_base
        {
            default_execution_context();

            void yield(const char* desc);
            void yield_k(std::size_t k, const char* desc);
            void suspend(const char* desc);
            void resume(const char* desc);
            void abort(const char* desc);
            void sleep_for(hpx::util::steady_duration const& sleep_duration,
                const char* desc);
            void sleep_until(hpx::util::steady_time_point const& sleep_time,
                const char* desc);

        private:
            bool running_;
            bool aborted_;
            std::mutex mtx_;
            std::condition_variable suspend_cv_;
            std::condition_variable resume_cv_;
        };

        default_execution_context::default_execution_context()
          : running_(true)
          , aborted_(false)
        {
        }

        void default_execution_context::yield(const char* desc)
        {
#if defined(HPX_SMT_PAUSE)
            HPX_SMT_PAUSE;
#else
#if defined(HPX_WINDOWS)
            Sleep(0);
#else
            sched_yield();
#endif
#endif
        }

        void default_execution_context::yield_k(std::size_t k, const char* desc)
        {
            if (k < 4)    //-V112
            {
            }
#if defined(HPX_SMT_PAUSE)
            else if (k < 16)
            {
                HPX_SMT_PAUSE;
            }
#endif
            else if (k < 32 || k & 1)    //-V112
            {
#if defined(HPX_WINDOWS)
                Sleep(0);
#else
                sched_yield();
#endif
            }
            else
            {
#if defined(HPX_WINDOWS)
                Sleep(1);
#else
                // g++ -Wextra warns on {} or {0}
                struct timespec rqtp = {0, 0};

                // POSIX says that timespec has tv_sec and tv_nsec
                // But it doesn't guarantee order or placement

                rqtp.tv_sec = 0;
                rqtp.tv_nsec = 1000;

                nanosleep(&rqtp, nullptr);
#endif
            }
        }

        void default_execution_context::suspend(const char* desc)
        {
            std::unique_lock<std::mutex> l(mtx_);
            HPX_ASSERT(running_);

            running_ = false;
            resume_cv_.notify_all();

            while (!running_)
            {
                suspend_cv_.wait(l);
            }

            if (aborted_)
            {
                std::ostringstream strm;
                strm << "std::thread(" << std::this_thread::get_id()
                    << ") aborted (yield returned wait_abort)";
                HPX_THROW_EXCEPTION(yield_aborted, "suspend", 
                    strm.str());
            }
        }

        void default_execution_context::resume(const char* desc)
        {
            {
                std::unique_lock<std::mutex> l(mtx_);
                while (running_)
                {
                    resume_cv_.wait(l);
                }
                running_ = true;
            }
            suspend_cv_.notify_one();
        }

        void default_execution_context::abort(const char* desc)
        {
            {
                std::unique_lock<std::mutex> l(mtx_);
                while (running_)
                {
                    resume_cv_.wait(l);
                }
                running_ = true;
                aborted_ = true;
            }
            suspend_cv_.notify_one();
        }

        void default_execution_context::sleep_for(
            hpx::util::steady_duration const& sleep_duration, const char* desc)
        {
            std::this_thread::sleep_for(sleep_duration.value());
        }

        void default_execution_context::sleep_until(
            hpx::util::steady_time_point const& sleep_time, const char* desc)
        {
            std::this_thread::sleep_until(sleep_time.value());
        }

        execution_context_base& default_context()
        {
            static thread_local default_execution_context context;
            return context;
        }
    }    // namespace

    namespace this_thread {

        namespace detail {
            struct execution_context_storage
            {
                execution_context_storage()
                  : impl_(&default_context())
                {
                }

                execution_context_base* set(
                    execution_context_base* context) noexcept
                {
                    std::swap(context, impl_);
                    return context;
                }

                execution_context_base* impl_;
            };

            execution_context_storage* get_execution_context_storage()
            {
                static thread_local execution_context_storage storage;
                return &storage;
            }
        }    // namespace detail

        reset_execution_context::reset_execution_context(
            detail::execution_context_storage* storage,
            execution_context_base& impl)
          : storage_(storage)
          , old_(storage_->set(&impl))
        {
        }

        reset_execution_context::reset_execution_context(
            execution_context_base& impl)
          : reset_execution_context(
                detail::get_execution_context_storage(), impl)
        {
        }

        reset_execution_context::~reset_execution_context()
        {
            storage_->set(old_);
        }

        hpx::execution::execution_context execution_context()
        {
            return hpx::execution::execution_context(
                detail::get_execution_context_storage()->impl_);
        }

        void yield(const char* desc)
        {
            execution_context().yield(desc);
        }

        void yield_k(std::size_t k, const char* desc)
        {
            execution_context().yield_k(k, desc);
        }

        void suspend(const char* desc)
        {
            execution_context().suspend(desc);
        }
    }    // namespace this_thread
}}       // namespace hpx::execution
