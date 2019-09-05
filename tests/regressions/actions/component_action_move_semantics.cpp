//  Copyright (c) 2007-2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/config/compiler_specific.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/include/async.hpp>
#include <hpx/include/plain_actions.hpp>
#include <hpx/testing.hpp>

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <tests/regressions/actions/components/action_move_semantics.hpp>
#include <tests/regressions/actions/components/movable_objects.hpp>

using hpx::test::movable_object;
using hpx::test::non_movable_object;

///////////////////////////////////////////////////////////////////////////////
template <typename Action, typename Object>
std::size_t pass_object(hpx::naming::id_type id)
{
    using hpx::test::action_move_semantics;

    action_move_semantics test = action_move_semantics::create(id);

    Object obj;
    obj.reset_count();

    return hpx::async<Action>(test.get_id(), obj).get();
}

///////////////////////////////////////////////////////////////////////////////
template <typename Action, typename Object>
std::size_t move_object(hpx::naming::id_type id)
{
    using hpx::test::action_move_semantics;

    action_move_semantics test = action_move_semantics::create(id);

    Object obj;
    obj.reset_count();

    return hpx::async<Action>(test.get_id(), std::move(obj)).get();
}

///////////////////////////////////////////////////////////////////////////////
template <typename Action, typename Object>
std::size_t return_object(hpx::naming::id_type id)
{
    using hpx::test::action_move_semantics;

    action_move_semantics test = action_move_semantics::create(id);

    Object obj(hpx::async<Action>(test.get_id()).get());
    return obj.get_count();
}

///////////////////////////////////////////////////////////////////////////////
template <typename Action, typename Object>
std::size_t return_move_object(hpx::naming::id_type id)
{
    using hpx::test::action_move_semantics;

    action_move_semantics test = action_move_semantics::create(id);

    Object obj(std::move(hpx::async<Action>(test.get_id()).move_out()));
    return obj.get_count();
}

///////////////////////////////////////////////////////////////////////////////
void test_actions()
{
    using hpx::test::server::action_move_semantics;

    std::vector<hpx::naming::id_type> localities = hpx::find_all_localities();
    for (hpx::naming::id_type const& id : localities)
    {
        bool is_local = id == hpx::find_here();

        // test std::size_t(movable_object const& obj)
        if (is_local)
        {
            HPX_TEST_EQ((pass_object<action_move_semantics::test_movable_action,
                            movable_object>(id)),
                1u);    // bind

            HPX_TEST_EQ((move_object<action_move_semantics::test_movable_action,
                            movable_object>(id)),
                0u);
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            HPX_TEST_EQ((pass_object<action_move_semantics::test_movable_action,
                            movable_object>(id)),
                1u);    // transfer_action

            HPX_TEST_EQ((move_object<action_move_semantics::test_movable_action,
                            movable_object>(id)),
                0u);
        }
#endif

        // test std::size_t(non_movable_object const& obj)
        if (is_local)
        {
            HPX_TEST_EQ(
                (pass_object<action_move_semantics::test_non_movable_action,
                    non_movable_object>(id)),
                2u);    // bind + function

            HPX_TEST_EQ(
                (move_object<action_move_semantics::test_non_movable_action,
                    non_movable_object>(id)),
                2u);    // bind + function
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            HPX_TEST_EQ(
                (pass_object<action_move_semantics::test_non_movable_action,
                    non_movable_object>(id)),
                3u);    // transfer_action + bind + function

            HPX_TEST_EQ(
                (move_object<action_move_semantics::test_non_movable_action,
                    non_movable_object>(id)),
                3u);    // transfer_action + bind + function
        }
#endif

        // test movable_object()
        if (is_local)
        {
            HPX_TEST_EQ((return_object<
                            action_move_semantics::return_test_movable_action,
                            movable_object>(id)),
                0u);
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            HPX_TEST_EQ((return_object<
                            action_move_semantics::return_test_movable_action,
                            movable_object>(id)),
                0u);
        }
#endif

        // test non_movable_object()
        if (is_local)
        {
            //FIXME: bumped number for intel compiler
            HPX_TEST_RANGE(
                (return_object<
                    action_move_semantics::return_test_non_movable_action,
                    non_movable_object>(id)),
                1u, 5u);    // ?call + set_value + ?return
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            //FIXME: bumped number for intel compiler
            HPX_TEST_RANGE(
                (return_object<
                    action_move_semantics::return_test_non_movable_action,
                    non_movable_object>(id)),
                3u, 8u);    // transfer_action + function + ?call +
                            // set_value + ?return
        }
#endif
    }
}

///////////////////////////////////////////////////////////////////////////////
void test_direct_actions()
{
    using hpx::test::server::action_move_semantics;

    std::vector<hpx::naming::id_type> localities = hpx::find_all_localities();
    for (hpx::naming::id_type const& id : localities)
    {
        bool is_local = id == hpx::find_here();

        // test std::size_t(movable_object const& obj)
        if (is_local)
        {
            HPX_TEST_EQ(
                (pass_object<action_move_semantics::test_movable_direct_action,
                    movable_object>(id)),
                0u);

            HPX_TEST_EQ(
                (move_object<action_move_semantics::test_movable_direct_action,
                    movable_object>(id)),
                0u);
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            HPX_TEST_EQ(
                (pass_object<action_move_semantics::test_movable_direct_action,
                    movable_object>(id)),
                1u);    // transfer_action

            HPX_TEST_EQ(
                (move_object<action_move_semantics::test_movable_direct_action,
                    movable_object>(id)),
                0u);
        }
#endif

        // test std::size_t(non_movable_object const& obj)
        if (is_local)
        {
            HPX_TEST_EQ(
                (pass_object<
                    action_move_semantics::test_non_movable_direct_action,
                    non_movable_object>(id)),
                0u);

            HPX_TEST_EQ(
                (move_object<
                    action_move_semantics::test_non_movable_direct_action,
                    non_movable_object>(id)),
                0u);
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            HPX_TEST_RANGE(
                (pass_object<
                    action_move_semantics::test_non_movable_direct_action,
                    non_movable_object>(id)),
                1u, 3u);    // transfer_action (if parcel is being routed)
                            // transfer_action, function + call (otherwise)

            HPX_TEST_RANGE(
                (move_object<
                    action_move_semantics::test_non_movable_direct_action,
                    non_movable_object>(id)),
                1u, 3u);    // transfer_action (if parcel is being routed)
                            // transfer_action, function + call (otherwise)
        }
#endif

        // test movable_object()
        if (is_local)
        {
            HPX_TEST_EQ(
                (return_object<
                    action_move_semantics::return_test_movable_direct_action,
                    movable_object>(id)),
                0u);
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            HPX_TEST_EQ(
                (return_object<
                    action_move_semantics::return_test_movable_direct_action,
                    movable_object>(id)),
                0u);
        }
#endif

        // test non_movable_object()
        if (is_local)
        {
            HPX_TEST_RANGE(
                (return_object<action_move_semantics::
                                   return_test_non_movable_direct_action,
                    non_movable_object>(id)),
                1u, 3u);    // ?call + set_value + ?return
        }
#if defined(HPX_HAVE_NETWORKING)
        else
        {
            //FIXME: bumped number for intel compiler
            HPX_TEST_RANGE(
                (return_object<action_move_semantics::
                                   return_test_non_movable_direct_action,
                    non_movable_object>(id)),
                3u, 8u);    // transfer_action + function + ?call +
                            // set_value + ?return
        }
#endif
    }
}

///////////////////////////////////////////////////////////////////////////////
int hpx_main(hpx::program_options::variables_map&)
{
    test_actions();
    test_direct_actions();

    hpx::finalize();
    return hpx::util::report_errors();
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // Configure application-specific options.
    hpx::program_options::options_description desc_commandline(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    // we need to explicitly enable the test components used by this test
    std::vector<std::string> const cfg = {
        "hpx.components.action_move_semantics.enabled! = 1"};

    // Initialize and run HPX.
    return hpx::init(desc_commandline, argc, argv, cfg);
}
