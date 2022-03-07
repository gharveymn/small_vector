/** test-move.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

struct explicitly_constructed
  : gch::test_types::nontrivial_data_base
{
  GCH_SMALL_VECTOR_TEST_CONSTEXPR explicit
  explicitly_constructed (int i) noexcept
    : gch::test_types::nontrivial_data_base (i)
  { }

  using gch::test_types::nontrivial_data_base::nontrivial_data_base;
};

template <typename T, typename Allocator = std::allocator<T>>
GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_type (Allocator alloc = Allocator ())
{
  // Insert at the beginning without reallocating.
  {
    gch::small_vector<T, 3, Allocator> v ({ T (2), T (3) }, alloc);
    auto pos = v.insert (v.begin (), T (1));

    CHECK (v.begin () == pos);
    CHECK (T (1) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3) } == v);
    CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  }

  // Insert in the middle without reallocating.
  {
    gch::small_vector<T, 3, Allocator> v ({ T (1), T (3) }, alloc);
    auto pos = v.insert (std::next (v.begin ()), T (2));

    CHECK (std::next (v.begin ()) == pos);
    CHECK (T (2) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3) } == v);
    CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  }

  // Insert at the end without reallocating.
  {
    gch::small_vector<T, 3, Allocator> v ({ T (1), T (2) }, alloc);
    auto pos = v.insert (v.end (), T (3));

    CHECK (std::prev (v.end ()) == pos);
    CHECK (T (3) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3) } == v);
    CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  }

  // Insert at the beginning while reallocating.
  {
    gch::small_vector<T, 3, Allocator> v ({ T (2), T (3), T (4) }, alloc);
    auto pos = v.insert (v.begin (), T (1));

    CHECK (v.begin () == pos);
    CHECK (T (1) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4) } == v);
    CHECK (! v.inlined ());
  }

  // Insert in the middle while reallocating.
  {
    gch::small_vector<T, 3, Allocator> v ({ T (1), T (3), T (4) }, alloc);
    auto pos = v.insert (std::next (v.begin ()), T (2));

    CHECK (std::next (v.begin ()) == pos);
    CHECK (T (2) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4) } == v);
    CHECK (! v.inlined ());
  }

  // Insert at the end while reallocating.
  {
    gch::small_vector<T, 3, Allocator> v ({ T (1), T (2), T (3) }, alloc);
    auto pos = v.insert (v.end (), T (4));

    CHECK (std::prev (v.end ()) == pos);
    CHECK (T (4) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4) } == v);
    CHECK (! v.inlined ());
  }

  return 0;
}

static
int
test_exceptions (void)
{
  using namespace gch::test_types;

  // Ensure valid states after exceptions.
  // Cases (Exception when...):
  //   No reallocation:
  //     - Creation of the temporary (No change).                  (1)
  //     - Moving elements into uninitialized memory.              (2)
  //     - Shifting initialized elements to the right.             (3)
  //     - Assignment of the temporary.                            (4)
  //     - Construction of the element at the end (No change).     (5)
  //   Reallocation:
  //     - Allocation too large (No change).                       (6)
  //     - Construction of the element (No change).                (7)
  //     - Moving of elements before `pos`.                        (8)
  //     - Moving of elements after `pos`.                         (9)
  //     - Moving of elements after construction of one at the end (10)

  using vector_type = gch::small_vector<triggering_copy_and_move, 4, verifying_allocator<triggering_copy_and_move>>;

  // Throw upon creation of the temporary. (1)
  {
    vector_type v { 1, 3, 4 };
    vector_type v_save = v;

    global_exception_trigger ().push (0);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (std::next (v.begin ()), 2));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);
  }

  // Throw while moving elements into uninitialized memory. (2)
  {
    vector_type v { 1, 3, 4 };
    vector_type v_save = v;

    global_exception_trigger ().push (1);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (std::next (v.begin ()), 2));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);
  }

  // Throw while shifting elements to the right. (3)
  {
    vector_type v { 1, 3, 4 };

    global_exception_trigger ().push (2);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (std::next (v.begin ()), 2));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (4 == v.size ());
    CHECK (! v[0].is_moved);
    CHECK (! v[1].is_moved);
    CHECK (  v[2].is_moved);
    CHECK (! v[3].is_moved);
  }

  // Throw while moving temporary into the element location. (4)
  {
    vector_type v { 1, 3, 4 };

    global_exception_trigger ().push (3);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (std::next (v.begin ()), 2));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (4 == v.size ());
    CHECK (! v[0].is_moved);
    CHECK (  v[1].is_moved);
    CHECK (! v[2].is_moved);
    CHECK (! v[3].is_moved);
  }

  // Throw during construction of the element at the end. (5)
  {
    vector_type v { 1, 2, 3 };
    vector_type v_save = v;

    global_exception_trigger ().push (0);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (v.end (), 4));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);
  }

  // Throw because of a length error. (6)
  {
    gch::small_vector_with_allocator<std::int8_t, sized_allocator<std::int8_t, std::uint8_t>> w;
    CHECK (127U == w.max_size ());
    w.assign (w.max_size (), 1);

    auto w_save = w;

    GCH_TRY
    {
      EXPECT_THROW (w.insert (w.end (), 2));
    }
    GCH_CATCH (const std::length_error&)
    { }

    CHECK (w == w_save);

    GCH_TRY
    {
      EXPECT_THROW (w.insert (w.begin (), 2));
    }
    GCH_CATCH (const std::length_error&)
    { }

    CHECK (w == w_save);

    GCH_TRY
    {
      EXPECT_THROW (w.insert (std::next (w.begin ()), 2));
    }
    GCH_CATCH (const std::length_error&)
    { }

    CHECK (w == w_save);
  }

  // Throw during construction of the element. (7)
  {
    vector_type v { 1, 2, 4, 5 };
    vector_type v_save = v;

    global_exception_trigger ().push (0);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (v.end (), 6));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);

    global_exception_trigger ().push (0);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (std::next (v.begin (), 2), 3));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);
  }

  // Throw during the move of elements to the new allocation which are to the left of `pos`. (8)
  {
    vector_type v { 1, 2, 4, 5 };

    global_exception_trigger ().push (2);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (std::next (v.begin (), 2), 3));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (4 == v.size ());
    CHECK (  v[0].is_moved);
    CHECK (! v[1].is_moved);
    CHECK (! v[2].is_moved);
    CHECK (! v[3].is_moved);
  }

  // Throw during the move of elements to the new allocation which are to the right of `pos`. (9)
  {
    vector_type v { 1, 2, 4, 5 };

    global_exception_trigger ().push (4);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (std::next (v.begin (), 2), 3));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (4 == v.size ());
    CHECK (  v[0].is_moved);
    CHECK (  v[1].is_moved);
    CHECK (  v[2].is_moved);
    CHECK (! v[3].is_moved);
  }

  // Throw during the move of elements to the new allocation after construction of one element at
  // the end. (10)
  {
    vector_type v { 1, 2, 3, 4 };
    vector_type v_save = v;

    // Throw during copy of index 0.
    global_exception_trigger ().push (1);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (v.end (), 5));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);

    // Throw during copy of index 1.
    global_exception_trigger ().push (2);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (v.end (), 5));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);

    // Throw during copy of index 2.
    global_exception_trigger ().push (3);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (v.end (), 5));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);

    // Throw during copy of index 3.
    global_exception_trigger ().push (4);
    GCH_TRY
    {
      EXPECT_THROW (v.insert (v.end (), 5));
    }
    GCH_CATCH (const test_exception&)
    { }

    CHECK (v == v_save);
  }

  return 0;
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  using namespace gch::test_types;

  CHECK (0 == test_with_type<trivially_copyable_data_base> ());
  CHECK (0 == test_with_type<nontrivial_data_base> ());
  CHECK (0 == test_with_type<explicitly_constructed> ());

  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             sized_allocator<trivially_copyable_data_base, std::uint8_t>> ());

  CHECK (0 == test_with_type<nontrivial_data_base,
                             fancy_pointer_allocator<nontrivial_data_base>> ());

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             verifying_allocator<trivially_copyable_data_base>> ());

  CHECK (0 == test_with_type<nontrivial_data_base, verifying_allocator<nontrivial_data_base>> ());

  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             non_propagating_verifying_allocator<trivially_copyable_data_base>> ());

  CHECK (0 == test_with_type<nontrivial_data_base,
                             non_propagating_verifying_allocator<nontrivial_data_base>> ());
#endif

  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             allocator_with_id<trivially_copyable_data_base>> ());

  CHECK (0 == test_with_type<nontrivial_data_base,
                             allocator_with_id<nontrivial_data_base>> ());

  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             propogating_allocator_with_id<trivially_copyable_data_base>> ());

  CHECK (0 == test_with_type<nontrivial_data_base,
                             propogating_allocator_with_id<nontrivial_data_base>> ());

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
  CHECK (0 == test_exceptions ());
#endif

  return 0;
}
