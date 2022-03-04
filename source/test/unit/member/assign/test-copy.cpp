/** test-copy.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

template <typename T, typename Allocator>
struct tester
{
  tester (void) = default;

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  tester (const Allocator& lhs_alloc, const Allocator& rhs_alloc)
    : m_lhs_alloc (lhs_alloc),
      m_rhs_alloc (rhs_alloc)
  { }

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  int
  operator() (void)
  {
    //  Null assignment.
    check<3, 5> ({ },
                 { });

    // Assignment to only uninitialized space with (v.size () == 0).
    check<3, 5> ({ },
                 { 11, 22 });

    // Assignment to only initialized space (v.size () == w.size ()).
    check<3, 5> ({  1,  2 },
                 { 11, 22 });

    // Assignment to both initialized and uninitialized space (v.size () < w.size ()).
    check<3, 5> ({  1,  2 },
                 { 11, 22, 33 });

    // Assignment to only initialized space (w.size () < v.size ()).
    check<3, 5> ({  1,  2, 3 },
                 { 11, 22 });

    // Reallocate (with both inlined).
    check<3, 5> ({  1,  2,  3 },
                 { 11, 22, 33, 44 });

    // Reallocate (with both allocated).
    check<3, 5> ({  1,  2,  3,  4 },
                 { 11, 22, 33, 44, 55, 66 });

    // Reallocate (with v inlined-empty, w allocated).
    check<3, 5> ({ },
                 { 11, 22, 33, 44, 55, 66 });

    // Reallocate (with v inlined-with-elements, w allocated).
    check<3, 5> ({  1,  2 },
                 { 11, 22, 33, 44, 55, 66 });

    // Check with v allocated and w inline.
    check<3, 5> ({  1,  2, 3, 4 },
                 { 11, 22 });

    // Assign where w is allocated, but does not cause an allocation for v, and v is empty.
    check<5, 3> ({ },
                 { 11, 22, 33, 44 });

    // Assign where w is allocated, but does not cause an allocation for v, and v has elements.
    check<5, 3> ({  1,  2 },
                 { 11, 22, 33, 44 });

    return 0;
  }

private:
  template <unsigned N, unsigned M>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (std::initializer_list<T> vi, std::initializer_list<T> wi)
  {
    using vector_type_N = gch::small_vector<T, N, Allocator>;
    using vector_type_M = gch::small_vector<T, M, Allocator>;

    vector_type_N v_cmp (vi, m_lhs_alloc);
    vector_type_M w_cmp (wi, m_rhs_alloc);
    {
      // vector_type_M (wi) -> vector_type_N (vi)
      vector_type_N n (vi, m_lhs_alloc);
      vector_type_M m (wi, m_rhs_alloc);

      n.assign (m);
      CHECK (n == w_cmp);
    }
    {
      // vector_type_N (vi) -> vector_type_M (wi)
      vector_type_N n (vi, m_lhs_alloc);
      vector_type_M m (wi, m_rhs_alloc);

      m.assign (n);
      CHECK (m == v_cmp);
    }
    {
      // vector_type_M (vi) -> vector_type_N (wi)
      vector_type_N n (wi, m_lhs_alloc);
      vector_type_M m (vi, m_rhs_alloc);

      n.assign (m);
      CHECK (n == v_cmp);
    }
    {
      // vector_type_N (wi) -> vector_type_M (vi)
      vector_type_N n (wi, m_lhs_alloc);
      vector_type_M m (vi, m_rhs_alloc);

      m.assign (n);
      CHECK (m == w_cmp);
    }
  }

  Allocator m_lhs_alloc;
  Allocator m_rhs_alloc;
};

template <typename T, typename Allocator = std::allocator<T>>
GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_type (Allocator alloc_v = Allocator (), Allocator alloc_w = Allocator ())
{
  return tester<T, Allocator> { alloc_v, alloc_w } ();
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

  non_propagating_verifying_allocator<trivially_copyable_data_base> np_alloc_v (1);
  non_propagating_verifying_allocator<trivially_copyable_data_base> np_alloc_w (2);
  CHECK (0 == test_with_type<trivially_copyable_data_base> (np_alloc_v, np_alloc_w));
  CHECK (0 == test_with_type<trivially_copyable_data_base,
                             non_propagating_verifying_allocator<trivially_copyable_data_base>> ());

  non_propagating_verifying_allocator<nontrivial_data_base> np_alloc2_v (1);
  non_propagating_verifying_allocator<nontrivial_data_base> np_alloc2_w (2);
  CHECK (0 == test_with_type<nontrivial_data_base> (np_alloc2_v, np_alloc2_w));
  CHECK (0 == test_with_type<nontrivial_data_base,
                             non_propagating_verifying_allocator<nontrivial_data_base>> ());
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
