//  Copyright (c) 2019 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/execution.hpp>
#include <hpx/testing.hpp>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

std::size_t dummy_called = 0;

struct dummy_context : hpx::execution::context_base
{
    hpx::execution::resource_base const& resource() const
    {
        return resource_;
    }

    hpx::execution::resource_base resource_;
};

struct dummy_agent : hpx::execution::agent_base
{
    std::string description() const { return ""; }
    dummy_context const& context() const
    {
        return context_;
    }

    void yield(const char* desc)
    {
        ++dummy_called;
    }
    void yield_k(std::size_t k, const char* desc) {}
    void suspend(const char* desc) {}
    void resume(const char* desc) {}
    void abort(const char* desc) {}
    void sleep_for(
        hpx::util::steady_duration const& sleep_duration, const char* desc)
    {
    }
    void sleep_until(
        hpx::util::steady_time_point const& sleep_time, const char* desc)
    {
    }

    dummy_context context_;
};

void test_basic_functionality()
{
    // Test that execution context forwards properly and resetting works
    {
        HPX_TEST_EQ(dummy_called, 0u);
        {
            dummy_agent dummy;
            hpx::execution::this_thread::reset_agent ctx(dummy);
            hpx::execution::this_thread::yield();
        }

        HPX_TEST_EQ(dummy_called, 1u);

        hpx::execution::this_thread::yield();

        HPX_TEST_EQ(dummy_called, 1u);
    }

    // Test that we get different contexts in different threads...
    {
        auto context = hpx::execution::this_thread::agent();
        std::thread t([&context]() {
            HPX_TEST_NEQ(
                context, hpx::execution::this_thread::agent());
        });
        t.join();
    }
}

struct simple_spinlock
{
    simple_spinlock()
      : locked_(false)
    {
    }

    void lock()
    {
        while (locked_.test_and_set())
        {
            hpx::execution::this_thread::yield();
        }
    }

    void unlock()
    {
        locked_.clear();
    }

    std::atomic_flag locked_;
};

void test_yield()
{
    std::vector<std::thread> ts;
    simple_spinlock mutex;
    std::size_t counter = 0;
    std::size_t repetitions = 1'000;
    for (std::size_t i = 0; i != std::thread::hardware_concurrency() * 10; ++i)
    {
        ts.emplace_back([&mutex, &counter, repetitions]() {
            for (std::size_t repeat = 0; repeat != repetitions; ++repeat)
            {
                std::unique_lock<simple_spinlock> l(mutex);
                ++counter;
            }
        });
    }

    for (auto& t : ts)
        t.join();

    HPX_TEST_EQ(
        counter, std::thread::hardware_concurrency() * repetitions * 10);
}

void test_suspend_resume()
{
    std::mutex mtx;
    hpx::execution::agent_ref suspended;

    bool resumed = false;

    std::thread t1([&mtx, &suspended, &resumed]() {
        auto context = hpx::execution::this_thread::agent();
        {
            std::unique_lock<std::mutex> l(mtx);
            suspended = context;
        }
        context.suspend();
        resumed = true;
    });

    while (true)
    {
        std::unique_lock<std::mutex> l(mtx);
        if (suspended)
            break;
    }

    suspended.resume();

    t1.join();
    HPX_TEST(resumed);
}

void test_sleep()
{
    auto now = std::chrono::steady_clock::now();
    auto sleep_duration = std::chrono::milliseconds(100);
    hpx::execution::this_thread::sleep_for(sleep_duration);
    HPX_TEST(std::chrono::steady_clock::now() >= now + sleep_duration);

    auto sleep_time = sleep_duration * 2 + std::chrono::steady_clock::now();
    hpx::execution::this_thread::sleep_until(sleep_time);
    HPX_TEST(std::chrono::steady_clock::now() >= now + sleep_duration * 2);
}

int main()
{
    test_basic_functionality();
    test_yield();
    test_suspend_resume();
    test_sleep();

    return hpx::util::report_errors();
}
