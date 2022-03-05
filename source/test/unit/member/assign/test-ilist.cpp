/** test-ilist.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

#include <array>

template <typename T, typename Allocator>
struct tester
{
  template <unsigned K>
  using vector_type = gch::small_vector<T, K, Allocator>;

  tester (void) = default;

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  tester (const Allocator& alloc)
    : m_alloc (alloc)
  { }

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  int
  operator() (void)
  {
    check_no_inline ();
    check_equal_inline ();

    return 0;
  }

  template <unsigned K>
  class data_wrapper
  {
  public:
    using value_type = gch::small_vector<T, K, Allocator>;

    GCH_SMALL_VECTOR_TEST_CONSTEXPR
    data_wrapper (std::initializer_list<T> data)
      : m_data (data)
    { }

    GCH_SMALL_VECTOR_TEST_CONSTEXPR
    data_wrapper (std::initializer_list<T> data, void (*prepare)(value_type&))
      : m_data (data),
        m_prepare (prepare)
    { }

    GCH_SMALL_VECTOR_TEST_CONSTEXPR
    typename value_type::const_iterator
    begin (void) const noexcept
    {
      return m_data.begin ();
    }

    GCH_SMALL_VECTOR_TEST_CONSTEXPR
    typename value_type::const_iterator
    end (void) const noexcept
    {
      return m_data.end ();
    }

    GCH_SMALL_VECTOR_TEST_CONSTEXPR
    void
    operator() (value_type& v)
    {
      if (m_prepare)
        m_prepare (v);
    }

  private:
    // We use a small_vector to store the data so that we can test constexpr.
    value_type m_data;
    void (*m_prepare)(value_type&) = nullptr;
  };

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  int
  check_no_inline (void)
  {
    // Check vectors with no inline elements.
    check<0> ({ },      { });
    check<0> ({ },      { 11, 22 });
    check<0> ({ 1 },    { 11, 22 });
    check<0> ({ 1, 2 }, { 11, 22 });
    return 0;
  }

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  int
  check_equal_inline (void)
  {
    // Check vectors with the same number of inline elements.
    // Let N = 2, and let both vectors have N inline elements.
    // States to check:
    //   Combinations of (with repeats):
    //     Inlined:
    //       0 == K elements    (1)
    //       0 < K < N elements (2)
    //       N == K elements    (3)
    //     Allocated:
    //       0 == K elements    (4)
    //       0 < K < N elements (5)
    //       N == K elements    (6)
    //       N < K elements     (7)
    // (28 total cases).

    auto reserver = [](vector_type<2>& v) {
      v.reserve (3);
    };

    std::array<data_wrapper<2>, 7> ns {
      data_wrapper<2> { },
      { 1 },
      { 1, 2 },
      { { },         reserver },
      { { 1 },       reserver },
      { { 1, 2 },    reserver },
      { { 1, 2, 3 }, reserver },
    };

    for (std::size_t i = 0; i < ns.size (); ++i)
      check (ns[i], { });

    for (std::size_t i = 0; i < ns.size (); ++i)
      check (ns[i], { 1 });

    for (std::size_t i = 0; i < ns.size (); ++i)
      check (ns[i], { 1, 2 });

    for (std::size_t i = 0; i < ns.size (); ++i)
      check (ns[i], { 1, 2, 3 });

    return 0;
  }

private:
  template <unsigned N>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (data_wrapper<N> vi, std::initializer_list<T> wi)
  {
    vector_type<N> v_cmp (wi);
    {
      vector_type<N> v (vi.begin (), vi.end (), m_alloc);

      vi (v);

      v.assign (wi);
      CHECK (v == v_cmp);
    }
  }

  Allocator m_alloc;
};

template <typename T, typename Allocator = std::allocator<T>>
GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_type (Allocator alloc = Allocator ())
{
  return tester<T, Allocator> { alloc } ();
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

  return 0;
}
