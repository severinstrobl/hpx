//  Copyright (c) 2007-2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#include <hpx/futures/traits/future_traits.hpp>
#include <hpx/modules/memory.hpp>

#include <type_traits>
#include <utility>
#include <vector>

namespace hpx { namespace lcos {
    template <typename R>
    class future;
    template <typename R>
    class shared_future;

    namespace detail {
        template <typename Result>
        struct future_data_base;
    }
}}    // namespace hpx::lcos

namespace hpx { namespace traits {
    ///////////////////////////////////////////////////////////////////////////
    namespace detail {
        struct future_data_void
        {
        };

        template <typename Result>
        struct shared_state_ptr_result
        {
            using type = Result;
        };

        template <typename Result>
        struct shared_state_ptr_result<Result&>
        {
            using type = Result&;
        };

        template <>
        struct shared_state_ptr_result<void>
        {
            using type = future_data_void;
        };

        template <typename R>
        struct shared_state_ptr
        {
            using result_type = typename shared_state_ptr_result<R>::type;
            using type =
                hpx::intrusive_ptr<lcos::detail::future_data_base<result_type>>;
        };

        template <typename Future, typename Enable = void>
        struct shared_state_ptr_for
          : shared_state_ptr<typename traits::future_traits<Future>::type>
        {
        };

        template <typename Future>
        struct shared_state_ptr_for<Future const> : shared_state_ptr_for<Future>
        {
        };

        template <typename Future>
        struct shared_state_ptr_for<Future&> : shared_state_ptr_for<Future>
        {
        };

        template <typename Future>
        struct shared_state_ptr_for<Future&&> : shared_state_ptr_for<Future>
        {
        };

        template <typename Future>
        struct shared_state_ptr_for<std::vector<Future>>
        {
            using type =
                std::vector<typename shared_state_ptr_for<Future>::type>;
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename SharedState, typename Allocator>
        struct shared_state_allocator;
    }    // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Enable = void>
    struct is_shared_state : std::false_type
    {
    };

    template <typename R>
    struct is_shared_state<
        hpx::intrusive_ptr<lcos::detail::future_data_base<R>>> : std::true_type
    {
    };

    ///////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename T, typename Enable = void>
        struct future_access_customization_point;
    }

    template <typename T>
    struct future_access : detail::future_access_customization_point<T>
    {
    };

    template <typename R>
    struct future_access<lcos::future<R>>
    {
        template <typename SharedState>
        static lcos::future<R> create(
            hpx::intrusive_ptr<SharedState> const& shared_state)
        {
            return lcos::future<R>(shared_state);
        }

        template <typename T = void>
        static lcos::future<R> create(typename detail::shared_state_ptr_for<
            lcos::future<lcos::future<R>>>::type const& shared_state)
        {
            return lcos::future<lcos::future<R>>(shared_state);
        }

        template <typename SharedState>
        static lcos::future<R> create(
            hpx::intrusive_ptr<SharedState>&& shared_state)
        {
            return lcos::future<R>(std::move(shared_state));
        }

        template <typename T = void>
        static lcos::future<R> create(typename detail::shared_state_ptr_for<
            lcos::future<lcos::future<R>>>::type&& shared_state)
        {
            return lcos::future<lcos::future<R>>(std::move(shared_state));
        }

        template <typename SharedState>
        static lcos::future<R> create(
            SharedState* shared_state, bool addref = true)
        {
            return lcos::future<R>(
                hpx::intrusive_ptr<SharedState>(shared_state, addref));
        }

        HPX_FORCEINLINE static
            typename traits::detail::shared_state_ptr<R>::type const&
            get_shared_state(lcos::future<R> const& f)
        {
            return f.shared_state_;
        }

        HPX_FORCEINLINE static
            typename traits::detail::shared_state_ptr<R>::type::element_type*
            detach_shared_state(lcos::future<R>&& f)
        {
            return f.shared_state_.detach();
        }
    };

    template <typename R>
    struct future_access<lcos::shared_future<R>>
    {
        template <typename SharedState>
        static lcos::shared_future<R> create(
            hpx::intrusive_ptr<SharedState> const& shared_state)
        {
            return lcos::shared_future<R>(shared_state);
        }

        template <typename T = void>
        static lcos::shared_future<R> create(
            typename detail::shared_state_ptr_for<
                lcos::shared_future<lcos::future<R>>>::type const& shared_state)
        {
            return lcos::shared_future<lcos::future<R>>(shared_state);
        }

        template <typename SharedState>
        static lcos::shared_future<R> create(
            hpx::intrusive_ptr<SharedState>&& shared_state)
        {
            return lcos::shared_future<R>(std::move(shared_state));
        }

        template <typename T = void>
        static lcos::shared_future<R> create(
            typename detail::shared_state_ptr_for<
                lcos::shared_future<lcos::future<R>>>::type&& shared_state)
        {
            return lcos::shared_future<lcos::future<R>>(
                std::move(shared_state));
        }

        template <typename SharedState>
        static lcos::shared_future<R> create(
            SharedState* shared_state, bool addref = true)
        {
            return lcos::shared_future<R>(
                hpx::intrusive_ptr<SharedState>(shared_state, addref));
        }

        HPX_FORCEINLINE static
            typename traits::detail::shared_state_ptr<R>::type const&
            get_shared_state(lcos::shared_future<R> const& f)
        {
            return f.shared_state_;
        }

        HPX_FORCEINLINE static
            typename traits::detail::shared_state_ptr<R>::type::element_type*
            detach_shared_state(lcos::shared_future<R> const& f)
        {
            return f.shared_state_.get();
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename SharedState, typename Allocator>
    struct shared_state_allocator
      : detail::shared_state_allocator<SharedState, Allocator>
    {
    };
}}    // namespace hpx::traits
