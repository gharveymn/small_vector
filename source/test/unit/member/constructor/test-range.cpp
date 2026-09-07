/** test-range.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

#include <array>

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
    check<0> ({ });
    check<0> ({ 1 });
    check<0> ({ 1, 2 });
    check<0> ({ 1, 2, 3 });
    check<0> ({ 1, 2, 3, 4 });
    check<0> ({ 1, 2, 3, 4, 5 });

    check<1> ({ });
    check<1> ({ 1 });
    check<1> ({ 1, 2 });
    check<1> ({ 1, 2, 3 });
    check<1> ({ 1, 2, 3, 4 });
    check<1> ({ 1, 2, 3, 4, 5 });

    check<2> ({ });
    check<2> ({ 1 });
    check<2> ({ 1, 2 });
    check<2> ({ 1, 2, 3 });
    check<2> ({ 1, 2, 3, 4 });
    check<2> ({ 1, 2, 3, 4, 5 });

    check<3> ({ });
    check<3> ({ 1 });
    check<3> ({ 1, 2 });
    check<3> ({ 1, 2, 3 });
    check<3> ({ 1, 2, 3, 4 });
    check<3> ({ 1, 2, 3, 4, 5 });

    return 0;
  }

private:
  template <unsigned N, typename U = T,
            typename std::enable_if<std::is_base_of<triggering_base, U>::value
            >::type * = nullptr>
  void
  check (std::initializer_list<T> mi)
  {
    verify_basic_exception_safety ([&] {
      vector_type<N> n {
        make_triggering_it (mi.begin ()),
        make_triggering_it (mi.end ())
      };
    });

    verify_basic_exception_safety ([&] {
      vector_type<N> {
        make_triggering_it (mi.begin ()),
        make_triggering_it (mi.end ()),
        m_alloc
      };
    });

    verify_basic_exception_safety ([&] {
      vector_type<N> {
        make_triggering_it (make_input_it (mi.begin ())),
        make_triggering_it (make_input_it (mi.end ()))
      };
    });

    verify_basic_exception_safety ([&] {
      vector_type<N> {
        make_triggering_it (make_input_it (mi.begin ())),
        make_triggering_it (make_input_it (mi.end ())),
        m_alloc
      };
    });

    verify_basic_exception_safety ([&] {
      vector_type<N> {
        make_triggering_it (make_fwd_it (mi.begin ())),
        make_triggering_it (make_fwd_it (mi.end ()))
      };
    });

    verify_basic_exception_safety ([&] {
      vector_type<N> {
        make_triggering_it (make_fwd_it (mi.begin ())),
        make_triggering_it (make_fwd_it (mi.end ())),
        m_alloc
      };
    });
    
    vector_type<N> w { mi.begin (), mi.end () };
    
    verify_basic_exception_safety ([=] {
      vector_type<N> n {
        make_triggering_it (std::make_move_iterator (w.begin ())),
        make_triggering_it (std::make_move_iterator (w.end ()))
      };
    });

    verify_basic_exception_safety ([=, this] {
      vector_type<N> {
        make_triggering_it (std::make_move_iterator (w.begin ())),
        make_triggering_it (std::make_move_iterator (w.end ())),
        m_alloc
      };
    });

    verify_basic_exception_safety ([=] {
      vector_type<N> {
        make_triggering_it (make_input_it (std::make_move_iterator (w.begin ()))),
        make_triggering_it (make_input_it (std::make_move_iterator (w.end ())))
      };
    });

    verify_basic_exception_safety ([=, this] {
      vector_type<N> {
        make_triggering_it (make_input_it (std::make_move_iterator (w.begin ()))),
        make_triggering_it (make_input_it (std::make_move_iterator (w.end ()))),
        m_alloc
      };
    });

    verify_basic_exception_safety ([=] {
      vector_type<N> {
        make_triggering_it (make_fwd_it (std::make_move_iterator (w.begin ()))),
        make_triggering_it (make_fwd_it (std::make_move_iterator (w.end ())))
      };
    });

    verify_basic_exception_safety ([=, this] {
      vector_type<N> {
        make_triggering_it (make_fwd_it (std::make_move_iterator (w.begin ()))),
        make_triggering_it (make_fwd_it (std::make_move_iterator (w.end ()))),
        m_alloc
      };
    });
  }

  template <unsigned N, typename U = T,
            typename std::enable_if<! std::is_base_of<triggering_base, U>::value
            >::type * = nullptr>
  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  check (std::initializer_list<T> mi)
  {
    {
      vector_type<N> v (mi.begin (), mi.end ());
      CHECK (mi.size () == v.size () && std::equal (mi.begin (), mi.end (), v.begin ()));
    }
    {
      vector_type<N> v (mi.begin (), mi.end (), m_alloc);
      CHECK (mi.size () == v.size () && std::equal (mi.begin (), mi.end (), v.begin ()));
      verify_not_created_by_container_copy_construction (v.get_allocator ());
    }
    {
      vector_type<N> v (make_input_it (mi.begin ()), make_input_it (mi.end ()));
      CHECK (mi.size () == v.size () && std::equal (mi.begin (), mi.end (), v.begin ()));
    }
    {
      vector_type<N> v (make_input_it (mi.begin ()), make_input_it (mi.end ()), m_alloc);
      CHECK (mi.size () == v.size () && std::equal (mi.begin (), mi.end (), v.begin ()));
      verify_not_created_by_container_copy_construction (v.get_allocator ());
    }
    {
      vector_type<N> v (make_fwd_it (mi.begin ()), make_fwd_it (mi.end ()));
      CHECK (mi.size () == v.size () && std::equal (mi.begin (), mi.end (), v.begin ()));
    }
    {
      vector_type<N> v (make_fwd_it (mi.begin ()), make_fwd_it (mi.end ()), m_alloc);
      CHECK (mi.size () == v.size () && std::equal (mi.begin (), mi.end (), v.begin ()));
      verify_not_created_by_container_copy_construction (v.get_allocator ());
    }
  }

  Allocator m_alloc;
};

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR

static
void
test_length_exception (void)
{
  const std::vector<std::int8_t> w (129, 1);

  {
    using vec = gch::small_vector_with_allocator<
      std::int8_t,
      verifying_sized_allocator<std::int8_t, std::uint8_t>
    >;

    GCH_TRY
    {
      EXPECT_THROW (vec { w.begin (), w.end () });
    }
    GCH_CATCH (const std::length_error&)
    { }

    GCH_TRY
    {
      EXPECT_THROW (vec { make_input_it (w.begin ()), make_input_it (w.end ()) });
    }
    GCH_CATCH (const std::length_error&)
    { }

    GCH_TRY
    {
      EXPECT_THROW (vec { make_fwd_it (w.begin ()), make_fwd_it (w.end ()) });
    }
    GCH_CATCH (const std::length_error&)
    { }
  }
  {
    // Test where the inline capacity exceeds the maximum size of the allocator.
    using vec = gch::small_vector<
      std::int8_t,
      128,
      verifying_sized_allocator<std::int8_t, std::uint8_t>
    >;

    GCH_TRY
    {
      EXPECT_THROW (vec  { w.begin (), w.end () });
    }
    GCH_CATCH (const std::length_error&)
    { }

    GCH_TRY
    {
      EXPECT_THROW (vec { make_input_it (w.begin ()), make_input_it (w.end ()) });
    }
    GCH_CATCH (const std::length_error&)
    { }

    GCH_TRY
    {
      EXPECT_THROW (vec { make_fwd_it (w.begin ()), make_fwd_it (w.end ()) });
    }
    GCH_CATCH (const std::length_error&)
    { }
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
  test_with_allocator<tester, propagating_allocator_with_id> ();

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
  test_with_allocator<tester, verifying_allocator> ();
  test_with_allocator<tester, non_propagating_verifying_allocator> ();

  test_length_exception ();
#endif

  return 0;
}
