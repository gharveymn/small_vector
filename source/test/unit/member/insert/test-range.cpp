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

template <typename T, typename Iter, typename Allocator>
GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_iterator (Allocator alloc)
{
  T values[] {
    T (0),
    T (1),
    T (2),
    T (3),
    T (4),
    T (5),
    T (6),
  };

  Iter iters[] {
    Iter (&values[0]),
    Iter (&values[1]),
    Iter (&values[2]),
    Iter (&values[3]),
    Iter (&values[4]),
    Iter (&values[5]),
    Iter (&values[6]),
  };

  // Insert at the beginning without reallocating.
  {
    gch::small_vector<T, 4, Allocator> v ({ T (3), T (4) }, alloc);
    auto pos = v.insert (v.begin (), iters[1], iters[3]);

    CHECK (v.begin () == pos);
    CHECK (T (1) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4) } == v);
    CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  }

  // Insert in the middle without reallocating (only assign).
  {
    gch::small_vector<T, 5, Allocator> v ({ T (1), T (4), T (5) }, alloc);
    auto pos = v.insert (std::next(v.begin ()), iters[2], iters[4]);

    CHECK (std::next (v.begin ()) == pos);
    CHECK (T (2) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4), T (5) } == v);
    CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  }

  // Insert in the middle without reallocating (both assign and construct).
  {
    gch::small_vector<T, 4, Allocator> v ({ T (1), T (4) }, alloc);
    auto pos = v.insert (std::next(v.begin ()), iters[2], iters[4]);

    CHECK (std::next (v.begin ()) == pos);
    CHECK (T (2) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4) } == v);
    CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  }

  // Insert at the end without reallocating.
  {
    gch::small_vector<T, 4, Allocator> v ({ T (1), T (2) }, alloc);
    auto pos = v.insert (v.end (), iters[3], iters[5]);

    CHECK (std::next (v.begin (), 2) == pos);
    CHECK (T (3) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4) } == v);
    CHECK_IF_NOT_CONSTEXPR (v.inlined ());
  }

  // Insert at the beginning while reallocating.
  {
    gch::small_vector<T, 4, Allocator> v ({ T (3), T (4), T (5) }, alloc);
    auto pos = v.insert (v.begin (), iters[1], iters[3]);

    CHECK (v.begin () == pos);
    CHECK (T (1) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4), T (5) } == v);
    CHECK (! v.inlined ());
  }

  // Insert in the middle while reallocating.
  {
    gch::small_vector<T, 4, Allocator> v ({ T (1), T (4), T (5) }, alloc);
    auto pos = v.insert (std::next (v.begin ()), iters[2], iters[4]);

    CHECK (std::next (v.begin ()) == pos);
    CHECK (T (2) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4), T (5) } == v);
    CHECK (! v.inlined ());
  }

  // Insert at the end while reallocating.
  {
    gch::small_vector<T, 4, Allocator> v ({ T (1), T (2), T (3) }, alloc);
    auto pos = v.insert (v.end (), iters[4], iters[6]);

    CHECK (std::next (v.begin (), 3) == pos);
    CHECK (T (4) == *pos);
    CHECK (decltype (v) { T (1), T (2), T (3), T (4), T (5) } == v);
    CHECK (! v.inlined ());
  }

  return 0;
}

template <typename T, typename Allocator = std::allocator<T>>
GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_type (Allocator alloc = Allocator ())
{
  test_with_iterator<T, single_pass_iterator<T *>> (alloc);
  test_with_iterator<T, multi_pass_iterator<T *>> (alloc);
  test_with_iterator<T, T *> (alloc);
  return 0;
}

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
    for (auto num_init = 0; num_init <= v.max_size (); ++num_init)
    {
      v.assign (num_init, 1);
      const auto v_save = v;

      const std::vector<std::int8_t> w (static_cast<std::size_t> (v.max_size () - num_init + 1), 2);

      auto pos = v.begin ();
      do
      {
        GCH_TRY
        {
          EXPECT_THROW (v.insert (pos, w.begin (), w.end ()));
        }
        GCH_CATCH (const std::length_error&)
        { }

        CHECK (v == v_save);

        GCH_TRY
        {
          EXPECT_THROW (v.insert (pos, make_input_it (w.begin ()), make_input_it (w.end ())));
        }
        GCH_CATCH (const std::length_error&)
        { }

        CHECK (v == v_save);

        GCH_TRY
        {
          EXPECT_THROW (v.insert (pos, make_fwd_it (w.begin ()), make_fwd_it (w.end ())));
        }
        GCH_CATCH (const std::length_error&)
        { }

        CHECK (v == v_save);

      } while (pos++ != v.end ());
    }
  }
  {
    // Test where the inline capacity exceeds the maximum size of the allocator.
    gch::small_vector<std::int8_t, 128, verifying_sized_allocator<std::int8_t, std::uint8_t>> v;
    CHECK (127U == v.max_size ());
    v.assign (128, 1);
    const auto v_save = v;
    const std::vector<std::int8_t> w { 2 };

    auto pos = v.begin ();
    do
    {
      GCH_TRY
      {
        EXPECT_THROW (v.insert (pos, w.begin (), w.end ()));
      }
      GCH_CATCH (const std::length_error&)
      { }

      CHECK (v == v_save);
    } while (pos++ != v.end ());
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
        v.insert (
          v.end (),
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
        v.insert (
          v.end (),
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

template <typename T, typename Allocator>
struct exception_tester
{
  template <unsigned K>
  using vector_init_type = vector_initializer<T, K, Allocator>;

  template <unsigned K>
  using vector_type = gch::small_vector<T, K, Allocator>;

  using diff_ty = typename vector_type<0>::difference_type;

  exception_tester (void) = default;

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  exception_tester (const Allocator& alloc)
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
  template <unsigned N>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (const vector_init_type<N>& vi, std::initializer_list<T> wi)
  {
    for (diff_ty offset = 0; offset <= static_cast<diff_ty> (vi.size ()); ++offset)
      check (vi, offset, wi);
  }

  template <unsigned N, typename U = T,
            typename std::enable_if<std::is_base_of<triggering_ctor, U>::value
            >::type * = nullptr>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (const vector_init_type<N>& vi, diff_ty offset, std::initializer_list<T> wi)
  {
    verify_strong_exception_guarantee (
      [&](vector_type<N>& v) {
        v.insert (std::next (v.begin (), offset), wi.begin (), wi.end ());
      },
      vi,
      m_alloc);

    verify_strong_exception_guarantee (
      [&](vector_type<N>& v) {
        v.insert (
          std::next (v.begin (), offset),
          make_input_it (&*wi.begin ()),
          make_input_it (&*wi.end ())
        );
      },
      vi,
      m_alloc);

    verify_strong_exception_guarantee (
      [&](vector_type<N>& v) {
        v.insert (
          std::next (v.begin (), offset),
          make_fwd_it (&*wi.begin ()),
          make_fwd_it (&*wi.end ())
        );
      },
      vi,
      m_alloc);
  }

  template <unsigned N, typename U = T,
            typename std::enable_if<std::is_base_of<triggering_type, U>::value
            >::type * = nullptr>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (const vector_init_type<N>& vi, diff_ty offset, std::initializer_list<T> wi)
  {
    verify_basic_exception_safety (
      [&](vector_type<N>& v) {
        v.insert (
          std::next (v.begin (), offset),
          make_triggering_it (wi.begin ()),
          make_triggering_it (wi.end ())
        );
      },
      vi,
      m_alloc);

    verify_basic_exception_safety (
      [&](vector_type<N>& v) {
          v.insert (
            std::next (v.begin (), offset),
            make_triggering_it (make_input_it (wi.begin ())),
            make_triggering_it (make_input_it (wi.end ()))
          );
      },
      vi,
      m_alloc);

    verify_basic_exception_safety (
      [&](vector_type<N>& v) {
        v.insert (
          std::next (v.begin (), offset),
          make_triggering_it (make_fwd_it (wi.begin ())),
          make_triggering_it (make_fwd_it (wi.end ()))
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
  check (const vector_init_type<N>&, diff_ty, std::initializer_list<T>)
  {

  }

  Allocator m_alloc;
};

#endif

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
                             propagating_allocator_with_id<trivially_copyable_data_base>> ());

  CHECK (0 == test_with_type<nontrivial_data_base,
                             propagating_allocator_with_id<nontrivial_data_base>> ());

#ifdef GCH_SMALL_VECTOR_TEST_EXCEPTION_SAFETY_TESTING
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

  test_with_allocator<exception_tester, verifying_allocator> ();
#endif

  printf("Number of exceptions: %zu", test_exception::exception_id ());
  return 0;
}
