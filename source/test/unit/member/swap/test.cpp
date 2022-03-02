/** test.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

using namespace gch::test_types;

template <typename T, typename Allocator = std::allocator<T>>
GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_type (Allocator alloc_v = Allocator (), Allocator alloc_w = Allocator ())
{
  gch::small_vector<T, 4, Allocator> v (alloc_v);
  gch::small_vector<T, 4, Allocator> w (alloc_w);

  CHECK (v == w);
  v.swap (w);
  CHECK (v == w);

  // One empty (inlined), the other inlined.
  v.emplace_back (1);
  v.emplace_back (2);

  CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (w.inlined ());

  auto save_v = v;
  auto save_w = w;

  v.swap (w);
  CHECK (v == save_w);
  CHECK (w == save_v);

  w.swap (v);
  CHECK (v == save_v);
  CHECK (w == save_w);

  // Both inlined.

  w.emplace_back (11);
  w.emplace_back (22);
  w.emplace_back (33);

  CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (w.inlined ());

  CHECK (v.size () == 2);
  CHECK (w.size () == 3);

  save_v = v;
  save_w = w;

  v.swap (w);
  CHECK (v == save_w);
  CHECK (w == save_v);

  w.swap (v);
  CHECK (v == save_v);
  CHECK (w == save_w);

  // One inlined.

  v.emplace_back (3);
  v.emplace_back (4);
  v.emplace_back (5);

  CHECK (v.size () == 5);
  CHECK (w.size () == 3);

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (w.inlined ());

  save_v = v;
  save_w = w;

  v.swap (w);
  CHECK (v == save_w);
  CHECK (w == save_v);

  w.swap (v);
  CHECK (v == save_v);
  CHECK (w == save_w);

  // Neither inlined.

  w.emplace_back (44);
  w.emplace_back (55);
  w.emplace_back (66);

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (! w.inlined ());

  save_v = v;
  save_w = w;

  v.swap (w);
  CHECK (v == save_w);
  CHECK (w == save_v);

  w.swap (v);
  CHECK (v == save_v);
  CHECK (w == save_w);

  // Neither inlined, with different lengths, and with insufficient space to swap element-wise.
  w.emplace_back (77);
  v.shrink_to_fit ();

  CHECK (v.capacity () < w.size ());

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (! w.inlined ());

  save_v = v;
  save_w = w;

  v.swap (w);
  CHECK (v == save_w);
  CHECK (w == save_v);

  w.swap (v);
  CHECK (v == save_v);
  CHECK (w == save_w);

  // One empty (not inlined), the other not inlined.
  v.clear ();

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (! w.inlined ());

  save_v = v;
  save_w = w;

  v.swap (w);
  CHECK (v == save_w);
  CHECK (w == save_v);

  w.swap (v);
  CHECK (v == save_v);
  CHECK (w == save_w);

  // One empty (inlined), the other not inlined.
  v.shrink_to_fit ();

  CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (! w.inlined ());

  save_v = v;
  save_w = w;

  v.swap (w);
  CHECK (v == save_w);
  CHECK (w == save_v);

  w.swap (v);
  CHECK (v == save_v);
  CHECK (w == save_w);

  // Both inlined, swap, and clear (check elements are being destroyed by correct allocator).
  v.clear ();
  v.shrink_to_fit ();
  w.clear ();
  w.shrink_to_fit ();

  v.emplace_back (1);
  v.emplace_back (2);

  w.emplace_back (11);
  w.emplace_back (22);

  v.swap (w);
  v.clear ();
  w.clear ();

  return 0;
}

template <typename T, typename Allocator = std::allocator<T>>
int
test_exceptions_with_type (Allocator alloc_v = Allocator (), Allocator alloc_w = Allocator ())
{
  gch::small_vector<T, 4, Allocator> v (alloc_v);
  gch::small_vector<T, 4, Allocator> v_save (alloc_v);

  gch::small_vector<T, 4, Allocator> w (alloc_w);
  gch::small_vector<T, 4, Allocator> w_save (alloc_w);

  // This is a *MASSIVE* kludge to get coverage on all try-catch blocks in
  // the swap functions. It basically is only looking for segfaults, rather
  // than assertions.
  auto run = [&](bool do_shrink = true) {
    v_save = v;
    w_save = w;

    gch::small_vector<std::size_t, 3> ec;
    while (ec.size () < 4)
    {
      GCH_TRY
      {
        for (std::size_t i : ec)
          global_exception_trigger.push (i);
        v.swap (w);
      }
      GCH_CATCH (const test_exception&)
      { }

      global_exception_trigger.reset ();
      v = v_save;
      w = w_save;
      if (do_shrink)
      {
        v.shrink_to_fit ();
        w.shrink_to_fit ();
      }

      GCH_TRY
      {
        for (std::size_t i : ec)
          global_exception_trigger.push (i);
        w.swap (v);
      }
      GCH_CATCH (const test_exception&)
      { }

      global_exception_trigger.reset ();
      v = v_save;
      w = w_save;
      if (do_shrink)
      {
        v.shrink_to_fit ();
        w.shrink_to_fit ();
      }

      if (ec.empty ())
        ec.push_back (0);
      else
      {
        if (++ec.back () == 15)
        {
          auto found = std::find_if_not (ec.rbegin (), ec.rend (), [](auto e) { return e == 15; });
          if (found == ec.rend ())
          {
            std::for_each (ec.rbegin (), found, [](auto& e) { e = 0; });
            ec.push_back (0);
          }
          else
          {
            ++*found;
            std::for_each (ec.rbegin (), found, [](auto& e) { e = 0; });
          }
        }
      }
    }
  };

  global_exception_trigger.reset ();
  v.assign ({ 1 });
  w.assign ({ });

  for (std::size_t i = 0; i < 4; ++i)
  {
    GCH_TRY
    {
      global_exception_trigger.push (i);
      v.swap (w);
    }
    GCH_CATCH (const test_exception&)
    { }

    global_exception_trigger.reset ();
    v = v_save;
    w = w_save;

    GCH_TRY
    {
      global_exception_trigger.push (i);
      w.swap (v);
    }
    GCH_CATCH (const test_exception&)
    { }

    global_exception_trigger.reset ();
    v = v_save;
    w = w_save;
  }

  global_exception_trigger.reset ();
  v.assign ({ 1 });
  w.assign ({ 11 });

  run ();

  global_exception_trigger.reset ();
  v.assign ({ 1, 2 });
  w.assign ({ 11, 22 });

  run ();

  global_exception_trigger.reset ();
  v.assign ({ 1, 2, 3 });
  w.assign ({ 11, 22 });

  run ();

  global_exception_trigger.reset ();
  v.assign ({1, 2, 3, 4, 5});
  w.assign ({11, 22});

  run ();

  global_exception_trigger.reset ();
  v.assign ({1, 2, 3, 4, 5});
  w.assign ({11, 22, 33, 44, 55, 66});

  run ();

  global_exception_trigger.reset ();
  v.assign ({1, 2, 3, 4, 5});
  w.assign ({11, 22, 33, 44, 55, 66});

  run (false);

  return 0;
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  {
    CHECK (0 == test_with_type<trivially_copyable_data_base> ());
    CHECK (0 == test_with_type<nontrivial_data_base> ());

    CHECK (0 == test_with_type<trivially_copyable_data_base,
                               sized_allocator<trivially_copyable_data_base, std::uint8_t>> ());

    CHECK (0 == test_with_type<nontrivial_data_base,
                               fancy_pointer_allocator<nontrivial_data_base>> ());

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
    verifying_allocator<trivially_copyable_data_base> alloc_v (1);
    verifying_allocator<trivially_copyable_data_base> alloc_w (2);
    CHECK (0 == test_with_type<trivially_copyable_data_base> (alloc_v, alloc_w));

    verifying_allocator<nontrivial_data_base> nontrivial_alloc_v (1);
    verifying_allocator<nontrivial_data_base> nontrivial_alloc_w (2);
    CHECK (0 == test_with_type<nontrivial_data_base> (nontrivial_alloc_v, nontrivial_alloc_w));
#endif

    allocator_with_id<trivially_copyable_data_base> awi_alloc_v (1);
    allocator_with_id<trivially_copyable_data_base> awi_alloc_w (2);
    CHECK (0 == test_with_type<trivially_copyable_data_base> (awi_alloc_v, awi_alloc_w));
    CHECK (0 == test_with_type<trivially_copyable_data_base,
                               allocator_with_id<trivially_copyable_data_base>> ());

    allocator_with_id<nontrivial_data_base> nt_awi_alloc_v (1);
    allocator_with_id<nontrivial_data_base> nt_awi_alloc_w (2);
    CHECK (0 == test_with_type<nontrivial_data_base> (nt_awi_alloc_v, nt_awi_alloc_w));
    CHECK (0 == test_with_type<nontrivial_data_base,
                               allocator_with_id<nontrivial_data_base>> ());

    propogating_allocator_with_id <trivially_copyable_data_base> pawi_alloc_v (1);
    propogating_allocator_with_id <trivially_copyable_data_base> pawi_alloc_w (2);
    CHECK (0 == test_with_type<trivially_copyable_data_base> (pawi_alloc_v, pawi_alloc_w));
    CHECK (0 == test_with_type<trivially_copyable_data_base,
                               propogating_allocator_with_id<trivially_copyable_data_base>> ());

    propogating_allocator_with_id <nontrivial_data_base> nt_pawi_alloc_v (1);
    propogating_allocator_with_id <nontrivial_data_base> nt_pawi_alloc_w (2);
    CHECK (0 == test_with_type<nontrivial_data_base> (nt_pawi_alloc_v, nt_pawi_alloc_w));
    CHECK (0 == test_with_type<nontrivial_data_base,
                               propogating_allocator_with_id<nontrivial_data_base>> ());
  }

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
  {
    CHECK (0 == test_exceptions_with_type<triggering_move_ctor> ());
    CHECK (0 == test_exceptions_with_type<triggering_move_ctor> ());

    CHECK (0 == test_exceptions_with_type<triggering_move_ctor,
                               sized_allocator<triggering_move_ctor, std::uint8_t>> ());

    CHECK (0 == test_exceptions_with_type<triggering_move_ctor,
                               fancy_pointer_allocator<triggering_move_ctor>> ());

    verifying_allocator<triggering_move_ctor> alloc_v (1);
    verifying_allocator<triggering_move_ctor> alloc_w (2);
    CHECK (0 == test_exceptions_with_type<triggering_move_ctor> (alloc_v, alloc_w));

    allocator_with_id<triggering_move_ctor> awi_alloc_v (1);
    allocator_with_id<triggering_move_ctor> awi_alloc_w (2);
    CHECK (0 == test_exceptions_with_type<triggering_move_ctor> (awi_alloc_v, awi_alloc_w));
    CHECK (0 == test_exceptions_with_type<triggering_move_ctor,
                                          allocator_with_id<triggering_move_ctor>> ());

    propogating_allocator_with_id<triggering_move_ctor> pawi_alloc_v (1);
    propogating_allocator_with_id<triggering_move_ctor> pawi_alloc_w (2);
    CHECK (0 == test_exceptions_with_type<triggering_move_ctor> (pawi_alloc_v, pawi_alloc_w));
    CHECK (0 == test_exceptions_with_type<triggering_move_ctor,
                                          propogating_allocator_with_id<triggering_move_ctor>> ());
  }
#endif

  return 0;
}
