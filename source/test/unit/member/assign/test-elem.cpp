/** test-elem.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

template <typename T, typename Allocator = std::allocator<T>>
GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_type (Allocator alloc_v = Allocator ())
{
  gch::small_vector<T, 4, Allocator> v (alloc_v);

  CHECK (v.empty ());

  // Assign to empty.
  v.assign (3, 1);
  CHECK (3 == v.size ());
  CHECK (1 == v[0]);
  CHECK (1 == v[1]);
  CHECK (1 == v[2]);

  // Shrink partially filled inlined.
  v.assign (1, 2);
  CHECK (1 == v.size ());
  CHECK (2 == v[0]);

  // Shrink to empty partially filled inlined.
  v.assign (0, 3);
  CHECK (0 == v.size ());

  // Assign allocation to inlined empty.
  v.assign (v.inline_capacity () + 1, 4);
  CHECK (v.inline_capacity () + 1 == v.size ());
  CHECK (v.inline_capacity () + 1 == std::count (v.begin (), v.end (), 4));

  // Assign allocation to inlined.
  v.erase (std::next (v.begin ()), v.end ());
  v.shrink_to_fit ();
  CHECK_IF_NOT_CONSTEXPR (v.inlined ());

  v.assign (v.inline_capacity () + 1, 5);
  CHECK (v.inline_capacity () + 1 == v.size ());
  CHECK (v.inline_capacity () + 1 == std::count (v.begin (), v.end (), 5));

  return 0;
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  using namespace gch::test_types;

  CHECK (0 == test_with_type<trivially_copyable_data_base> ());
  CHECK (0 == test_with_type<nontrivial_data_base> ());

  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             sized_allocator<trivially_copyable_data_base, std::uint8_t>> ());

  CHECK (0 == test_with_type<nontrivial_data_base,
                             fancy_pointer_allocator<nontrivial_data_base>> ());

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             verifying_allocator<trivially_copyable_data_base>> ());
  CHECK (0 == test_with_type<nontrivial_data_base, verifying_allocator<nontrivial_data_base>> ());
#endif

  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             allocator_with_id<trivially_copyable_data_base>> ());
  CHECK (0 == test_with_type<nontrivial_data_base,
                             allocator_with_id<nontrivial_data_base>> ());

  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             propogating_allocator_with_id<trivially_copyable_data_base>> ());
  CHECK (0 == test_with_type<nontrivial_data_base,
                             propogating_allocator_with_id<nontrivial_data_base>> ());

  return 0;
}
