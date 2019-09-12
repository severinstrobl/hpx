//  Copyright (c) 2013-2019 Thomas Heller
//  Copyright (c) 2008 Peter Dimov
//  Copyright (c) 2018 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/assertion.hpp>
#include <hpx/errors/throw_exception.hpp>
#include <hpx/execution/agent_base.hpp>
#include <hpx/execution/this_thread.hpp>
#include <hpx/format.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <condition_variable>
#include <cstdint>
#include <mutex>
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
        struct HPX_EXPORT default_context : context_base
        {
            resource_base const& resource() const
            {
                return resource_;
            }
            resource_base resource_;
        };

        struct HPX_EXPORT default_agent : agent_base
        {
            default_agent();

            std::string description() const
            {
                return hpx::util::format("{}", id_);
            }

            default_context const& context() const
            {
                return context_;
            }

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
            std::thread::id id_;
            std::mutex mtx_;
            std::condition_variable suspend_cv_;
            std::condition_variable resume_cv_;

            default_context context_;
        };

        default_agent::default_agent()
          : running_(true)
          , aborted_(false)
          , id_(std::this_thread::get_id())
        {
        }

        void default_agent::yield(const char* desc)
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

        void default_agent::yield_k(std::size_t k, const char* desc)
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

        void default_agent::suspend(const char* desc)
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
                HPX_THROW_EXCEPTION(yield_aborted, "suspend", 
                    hpx::util::format("std::thread({}) aborted (yield returned wait_abort)",
                        id_));
            }
        }

        void default_agent::resume(const char* desc)
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

        void default_agent::abort(const char* desc)
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

        void default_agent::sleep_for(
            hpx::util::steady_duration const& sleep_duration, const char* desc)
        {
            std::this_thread::sleep_for(sleep_duration.value());
        }

        void default_agent::sleep_until(
            hpx::util::steady_time_point const& sleep_time, const char* desc)
        {
            std::this_thread::sleep_until(sleep_time.value());
        }

        agent_base& get_default_agent()
        {
            static thread_local default_agent agent;
            return agent;
        }
    }    // namespace

    namespace this_thread {

        namespace detail {
            struct agent_storage
            {
                agent_storage()
                  : impl_(&get_default_agent())
                {
                }

                agent_base* set(
                    agent_base* context) noexcept
                {
                    std::swap(context, impl_);
                    return context;
                }

                agent_base* impl_;
            };

            agent_storage* get_agent_storage()
            {
                static thread_local agent_storage storage;
                return &storage;
            }
        }    // namespace detail

        reset_agent::reset_agent(
            detail::agent_storage* storage,
            agent_base& impl)
          : storage_(storage)
          , old_(storage_->set(&impl))
        {
        }

        reset_agent::reset_agent(agent_base& impl)
          : reset_agent(
                detail::get_agent_storage(), impl)
        {
        }

        reset_agent::~reset_agent()
        {
            storage_->set(old_);
        }

        hpx::execution::agent agent()
        {
            return hpx::execution::agent(detail::get_agent_storage()->impl_);
        }

        void yield(const char* desc)
        {
            agent().yield(desc);
        }

        void yield_k(std::size_t k, const char* desc)
        {
            agent().yield_k(k, desc);
        }

        void suspend(const char* desc)
        {
            agent().suspend(desc);
        }
    }    // namespace this_thread
}}       // namespace hpx::execution
