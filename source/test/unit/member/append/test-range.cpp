/** test-range.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

#include <array>
#include <vector>

using namespace gch::test_types;

template <typename T, typename Allocator>
struct tester
{
  template <unsigned K>
  using vector_init_type = vector_initializer<T, K, Allocator>;

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

    auto reserver = [](vector_type<2>& v) {
      v.reserve (3);
    };

    std::array<vector_init_type<2>, 8> ns {
      vector_init_type<2> { },
      { 1 },
      { 1, 2 },
      { { },      reserver },
      { { 1 },    reserver },
      { { 1, 2 }, reserver },
      { { 1, 2, 3 }, },
      { { 1, 2, 3, 4 }, },
    };

    for (std::size_t i = 0; i < ns.size (); ++i)
    {
      check (ns[i], { });
      check (ns[i], { 1 });

      // Same number as the inline capacity (2)
      check (ns[i], { 1, 2 });

      // One more than the inline capacity (2)
      check (ns[i], { 1, 2, 3 });

      // Same number as the first allocated capacity (4)
      check (ns[i], { 1, 2, 3, 4 });

      // One more than the first allocated capacity (4)
      check (ns[i], { 1, 2, 3, 4, 5 });

      // This will exercise the recursion seen in the InputIterator algorithm.
      check (ns[i], { 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    }

    // Check vectors with no inline elements.
    check<0> ({ },      { });
    check<0> ({ 1 },    { });
    check<0> ({ 1, 2 }, { });
    check<0> ({ },      { 11 });
    check<0> ({ 1 },    { 11 });
    check<0> ({ 1, 2 }, { 11 });
    check<0> ({ },      { 11, 22 });
    check<0> ({ 1 },    { 11, 22 });
    check<0> ({ 1, 2 }, { 11, 22 });

    return 0;
  }

private:
  template <unsigned N, typename U = T,
            typename std::enable_if<std::is_base_of<triggering_ctor, U>::value
            >::type * = nullptr>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (vector_init_type<N> vi, std::initializer_list<T> wi)
  {
    using input_it = single_pass_iterator<const T *>;
    using forward_it = multi_pass_iterator<const T *>;

    verify_strong_exception_guarantee (
      [&](vector_type<N>& v) { v.append (wi.begin (), wi.end ()); },
      vi,
      m_alloc);

    verify_strong_exception_guarantee (
      [&](vector_type<N>& v) { v.append (input_it (wi.begin ()), input_it (wi.end ())); },
      vi,
      m_alloc);

    verify_strong_exception_guarantee (
      [&](vector_type<N>& v) { v.append (forward_it (wi.begin ()), forward_it (wi.end ())); },
      vi,
      m_alloc);
  }

  template <unsigned N, typename U = T,
            typename std::enable_if<std::is_base_of<triggering_type, U>::value
            >::type * = nullptr>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (vector_init_type<N> vi, std::initializer_list<T> wi)
  {
    using input_it = single_pass_iterator<const T *>;
    using forward_it = multi_pass_iterator<const T *>;

    verify_basic_exception_safety (
      [&](vector_type<N>& v) {
        v.append (
          make_triggering_it (wi.begin ()),
          make_triggering_it (wi.end ())
        );
      },
      vi,
      m_alloc);

    verify_basic_exception_safety (
      [&](vector_type<N>& v) {
        v.append (
          make_triggering_it (input_it (wi.begin ())),
          make_triggering_it (input_it (wi.end ()))
        );
      },
      vi,
      m_alloc);

    verify_basic_exception_safety (
      [&](vector_type<N>& v) {
        v.append (
          make_triggering_it (forward_it (wi.begin ())),
          make_triggering_it (forward_it (wi.end ()))
        );
      },
      vi,
      m_alloc);
  }

  template <unsigned N, typename U = T,
            typename std::enable_if<! std::is_base_of<triggering_base, U>::value
            >::type * = nullptr>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (vector_init_type<N> vi, std::initializer_list<T> wi)
  {
    using input_it = single_pass_iterator<const T *>;
    using forward_it = multi_pass_iterator<const T *>;

    vector_type<N> v_cmp (vi.begin (), vi.end ());
    for (const T& elem : wi)
      v_cmp.push_back (elem);

    {
      vector_type<N> v (vi.begin (), vi.end (), m_alloc);

      vi (v);

      v.append (wi.begin (), wi.end ());
      CHECK (v == v_cmp);
    }
    {
      vector_type<N> v (vi.begin (), vi.end (), m_alloc);

      vi (v);

      v.append (input_it (wi.begin ()), input_it (wi.end ()));
      CHECK (v == v_cmp);
    }
    {
      vector_type<N> v (vi.begin (), vi.end (), m_alloc);

      vi (v);

      v.append (forward_it (wi.begin ()), forward_it (wi.end ()));
      CHECK (v == v_cmp);
    }
  }

  Allocator m_alloc;
};


#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR

static
void
test_length_exception (void)
{
  {
    gch::small_vector_with_allocator<
      std::int8_t,
      verifying_sized_allocator<std::int8_t, std::uint8_t>
    > v;
    CHECK (127U == v.max_size ());
    for (std::uint8_t num_init = 0; num_init <= v.max_size (); ++num_init)
    {
      v.assign (num_init, 1);
      const auto v_save = v;

      const std::vector<std::int8_t> w (static_cast<std::size_t> (v.max_size () - num_init + 1), 2);

      GCH_TRY
      {
        EXPECT_THROW (v.append (w.begin (), w.end ()));
      }
      GCH_CATCH (const std::length_error&)
      { }

      CHECK (v == v_save);

      GCH_TRY
      {
        EXPECT_THROW (v.append (make_input_it (w.begin ()), make_input_it (w.end ())));
      }
      GCH_CATCH (const std::length_error&)
      { }

      CHECK (v == v_save);

      GCH_TRY
      {
        EXPECT_THROW (v.append (make_fwd_it (w.begin ()), make_fwd_it (w.end ())));
      }
      GCH_CATCH (const std::length_error&)
      { }

      CHECK (v == v_save);
    }
  }
  {
    // Test where the inline capacity exceeds the maximum size of the allocator.
    gch::small_vector<std::int8_t, 128, verifying_sized_allocator<std::int8_t, std::uint8_t>> v;
    v.assign (128, 1);
    const auto v_save = v;

    const std::vector<std::int8_t> w { 2 };
    GCH_TRY
    {
      EXPECT_THROW (v.append (w.begin (), w.end ()));
    }
    GCH_CATCH (const std::length_error&)
    { }

    CHECK (v == v_save);
  }
}


// This function is specifically testing that the following behavior is shown:
//
// > If an exception is thrown when inserting a single element at the end, and T is CopyInsertable
// > into `*this` or `std::is_nothrow_move_constructible<T>::value` is `true`, this function has no
// > effect (strong exception guarantee). Otherwise, if an exception is thrown by the move
// > constructor of a non-CopyInsertable T, the effects are unspecified.
template <typename T>
static
void
test_single_element_append_exceptions (bool strong)
{
  using vec = gch::small_vector_with_allocator<T, verifying_allocator<T>>;

  auto generator = [] {
    int i = 0;
    return [i]() mutable { return T { i++ }; };
  };

  for (std::size_t init_count = 0; init_count <= 2 * vec::inline_capacity_v; ++init_count)
  {
    verify_exception_stability (
      [](vec& v, vec& w) {
        v.append (
          make_triggering_it (std::make_move_iterator (make_input_it (w.begin ()))),
          make_triggering_it (std::make_move_iterator (std::next (make_input_it (w.begin ()))))
       );
      },
      strong,
      vec { init_count, generator () },
      [&] { return vec { init_count, generator () }; },
      [&] { return vec { 1, generator () }; }
    );

    verify_exception_stability (
      [](vec& v, vec& w) {
        v.append (
          make_triggering_it (std::make_move_iterator (make_fwd_it (w.begin ()))),
          make_triggering_it (std::make_move_iterator (std::next (make_fwd_it (w.begin ()))))
       );
      },
      strong,
      vec { init_count, generator () },
      [&] { return vec { init_count, generator () }; },
      [&] { return vec { 1, generator () }; }
    );
  }
}

#endif

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  using namespace gch::test_types;

  test_with_allocator<tester, std::allocator> ();
  test_with_allocator<tester, sized_allocator, std::uint8_t> ();
  test_with_allocator<tester, fancy_pointer_allocator> ();
  test_with_allocator<tester, allocator_with_id> ();

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR

  test_with_allocator<tester, verifying_allocator> ();

  test_length_exception ();

#ifdef GCH_LIB_CONCEPTS
  static_assert (
        gch::concepts::CopyInsertable<triggering_type, gch::small_vector<triggering_type>>
    &&! std::is_nothrow_move_constructible<triggering_type>::value
  );
#endif
  test_single_element_append_exceptions<triggering_type> (true);

#ifdef GCH_LIB_CONCEPTS
  static_assert (
      ! gch::concepts::CopyInsertable<triggering_noexcept_move_only, gch::small_vector<triggering_noexcept_move_only>>
    &&  std::is_nothrow_move_constructible<triggering_noexcept_move_only>::value
  );
#endif
  test_single_element_append_exceptions<triggering_noexcept_move_only> (true);

#ifdef GCH_LIB_CONCEPTS
  static_assert (
      ! gch::concepts::CopyInsertable<triggering_move_only, gch::small_vector<triggering_move_only>>
    &&! std::is_nothrow_move_constructible<triggering_move_only>::value
  );
#endif
  test_single_element_append_exceptions<triggering_move_only> (false);

#endif

  return 0;
}
