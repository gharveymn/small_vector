/** test-copy.cpp
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
test_with_type (Allocator alloc_v = Allocator (), Allocator alloc_w = Allocator ())
{
  gch::small_vector<T, 3, Allocator> v (alloc_v);
  gch::small_vector<T, 5, Allocator> w (alloc_w);

  auto save_v = v;
  auto save_w = w;

#define VERIFY_ALLOCATION(V)                    \
(save_ ## V).assign ((V).begin (), (V).end ()); \
(V).clear ();                                   \
(V).shrink_to_fit ();                           \
(V) = save_ ## V

#define CHECK_ASSIGN(V, W) \
(V).assign (W);            \
CHECK ((V) == (W));        \
VERIFY_ALLOCATION (V);     \
VERIFY_ALLOCATION (W)

  //  Null assignment.
  CHECK (v == w);

  v.assign (w);
  CHECK (v == w);

  CHECK_ASSIGN (v, w);

  // Assignment to only uninitialized space with (v.size () == 0).
  w.emplace_back (1);
  w.emplace_back (2);

  CHECK_ASSIGN (v, w);

  // Assignment to only initialized space (v.size () == w.size ()).
  w[0] = 11;
  w[1] = 22;

  CHECK_ASSIGN (v, w);

  // Assignment to both initialized and uninitialized space (v.size () < w.size ()).
  w[0] = 1;
  w[1] = 2;
  w.emplace_back (3);

  CHECK_ASSIGN (v, w);

  // Assignment to only initialized space (w.size () < v.size ()).
  w.pop_back ();
  CHECK (w.size () < v.size ());

  CHECK_ASSIGN (v, w);

  // Reallocate (with both inlined).
  w.emplace_back (3);
  w.emplace_back (4);
  CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (w.inlined ());
  CHECK (4 == w.size ());

  CHECK_ASSIGN (v, w);

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());

  // Reallocate (with both allocated).
  for (std::size_t i = w.size (); i < v.capacity () + 1U; ++i)
    w.emplace_back (static_cast<int> (i));

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (! w.inlined ());
  CHECK (w.size () == v.capacity () + 1);

  CHECK_ASSIGN (v, w);

  // Reallocate (with v inlined-empty, w allocated).
  v.clear ();
  v.shrink_to_fit ();

  CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (! w.inlined ());

  CHECK_ASSIGN (v, w);

  // Reallocate (with v inlined-with-elements, w allocated).
  v.clear ();
  v.shrink_to_fit ();
  v.emplace_back (11);
  v.emplace_back (22);

  CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (! w.inlined ());

  CHECK (v[0] != w[0]);
  CHECK (v[1] != w[1]);

  CHECK_ASSIGN (v, w);

  // Check with v allocated and w inline.
  w.clear ();
  w.shrink_to_fit ();
  w.emplace_back (1);
  w.emplace_back (2);

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (w.inlined ());

  CHECK_ASSIGN (v, w);

  // Assign v to w where v is allocated, but does not cause an allocation for w, and w is empty.
  v.clear ();
  v.shrink_to_fit ();
  v.emplace_back (1);
  v.emplace_back (2);
  v.emplace_back (3);
  v.emplace_back (4);

  w.clear ();
  w.shrink_to_fit ();

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (w.inlined ());

  CHECK_ASSIGN (w, v);

  // Assign v to w where v is allocated, but does not cause an allocation for w, and w has elements.
  w.clear ();
  w.shrink_to_fit ();
  w.emplace_back (11);
  w.emplace_back (22);

  CHECK (v[0] != w[0]);
  CHECK (v[1] != w[1]);

  CHECK_IF_NOT_CONSTEXPR (! v.inlined ());
  CHECK_IF_NOT_CONSTEXPR (w.inlined ());

  CHECK_ASSIGN (w, v);

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
  verifying_allocator<trivially_copyable_data_base> alloc_v (1);
  verifying_allocator<trivially_copyable_data_base> alloc_w (2);
  CHECK (0 == test_with_type<trivially_copyable_data_base> (alloc_v, alloc_w));
  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             verifying_allocator<trivially_copyable_data_base>> ());

  verifying_allocator<nontrivial_data_base> nontrivial_alloc_v (1);
  verifying_allocator<nontrivial_data_base> nontrivial_alloc_w (2);
  CHECK (0 == test_with_type<nontrivial_data_base> (nontrivial_alloc_v, nontrivial_alloc_w));
  CHECK (0 == test_with_type<nontrivial_data_base, verifying_allocator<nontrivial_data_base>> ());
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

  propogating_allocator_with_id<trivially_copyable_data_base> pawi_alloc_v (1);
  propogating_allocator_with_id<trivially_copyable_data_base> pawi_alloc_w (2);
  CHECK (0 == test_with_type<trivially_copyable_data_base> (pawi_alloc_v, pawi_alloc_w));
  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             propogating_allocator_with_id<trivially_copyable_data_base>> ());

  propogating_allocator_with_id<nontrivial_data_base> nt_pawi_alloc_v (1);
  propogating_allocator_with_id<nontrivial_data_base> nt_pawi_alloc_w (2);
  CHECK (0 == test_with_type<nontrivial_data_base> (nt_pawi_alloc_v, nt_pawi_alloc_w));
  CHECK (0 == test_with_type<nontrivial_data_base,
                             propogating_allocator_with_id<nontrivial_data_base>> ());

  return 0;
}
