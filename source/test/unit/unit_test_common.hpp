/** unit_test_common.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SMALL_VECTOR_UNIT_TEST_COMMON_HPP
#define SMALL_VECTOR_UNIT_TEST_COMMON_HPP

#include "test_common.hpp"
#include "test_types.hpp"
#include "test_allocators.hpp"

#include <vector>

template <typename T, unsigned N, typename Allocator>
class vector_initializer
{
public:
  using value_type = gch::small_vector<T, N, Allocator>;

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  vector_initializer (std::initializer_list<T> data)
    : m_data (data)
  { }

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  vector_initializer (std::initializer_list<T> data, void (*prepare)(value_type&))
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
  typename value_type::size_type
  size (void) const noexcept
  {
    return static_cast<typename value_type::size_type> (
      std::distance (m_data.begin (), m_data.end ())
    );
  }

  GCH_SMALL_VECTOR_TEST_CONSTEXPR
  void
  operator() (value_type& v) const
  {
    if (m_prepare)
      m_prepare (v);
  }

private:
  // We use a small_vector to store the data so that we can test constexpr.
  value_type m_data;
  void (* m_prepare) (value_type&) = nullptr;
};

template <typename Functor, typename ...Args>
bool
trigger_exceptions (Functor& f, std::vector<std::size_t>& test_counts, Args&&... args)
{
  using namespace gch::test_types;

  std::for_each (test_counts.rbegin (), test_counts.rend (), [&](std::size_t c) {
    exception_trigger::push (c);
  });

  GCH_TRY
  {
    f (std::forward<Args> (args)...);
  }
  GCH_CATCH (const test_exception&)
  { }

  if (exception_trigger::has_pending_throws ())
  {
    // This means that there weren't enough opportunities for the exception trigger queue to pop
    // before the function finished. So, we are done.
    exception_trigger::reset ();
    test_counts.pop_back ();
    if (! test_counts.empty ())
      ++test_counts.back ();
    return false;
  }

  if (0 != exception_trigger::extra_test_count ())
    test_counts.push_back (0);
  else
    ++test_counts.back ();

  return true;
}

template <typename Functor, typename Cmp, typename Generator>
void
verify_exception_stability (Functor&& f, bool strong, Cmp v_cmp, Generator gen)
{
  using namespace gch::test_types;

  std::vector<std::size_t> test_counts;
  test_counts.push_back (0);
  do
  {
    verifying_allocator_base::with_scoped_context([&] {
      auto v = gen ();

      bool threw = trigger_exceptions (f, test_counts, v);

      if (strong && threw)
        CHECK (v == v_cmp);
    });
  } while (! test_counts.empty ());
}

template <typename Functor, typename T, unsigned N, typename Allocator>
void
verify_exception_stability (
  Functor f,
  bool strong,
  vector_initializer<T, N, Allocator> vi,
  Allocator alloc)
{
  verify_exception_stability (
    f,
    strong,
    gch::small_vector<T, N, Allocator> { vi.begin (), vi.end (), alloc },
    [&]() -> gch::small_vector<T, N, Allocator> {
      gch::small_vector<T, N, Allocator> v { vi.begin (), vi.end (), alloc };
      vi (v);
      return v;
    }
  );
}

template <typename Functor, typename Cmp, typename Generator1, typename Generator2>
void
verify_exception_stability (Functor&& f, bool strong, Cmp v_cmp, Generator1 gen1, Generator2 gen2)
{
  using namespace gch::test_types;

  std::vector<std::size_t> test_counts;
  test_counts.push_back (0);
  do
  {
    verifying_allocator_base::with_scoped_context([&] {
      auto v = gen1 ();
      auto w = gen2 ();

      bool threw = trigger_exceptions (f, test_counts, v, w);

      if (strong && threw)
        CHECK (v == v_cmp);
    });
  } while (! test_counts.empty ());
}

template <typename Functor,
          typename T,
          unsigned N,
          unsigned M,
          typename Allocator>
void
verify_exception_stability (
  Functor f,
  bool strong,
  vector_initializer<T, N, Allocator> ni,
  vector_initializer<T, M, Allocator> mi,
  Allocator alloc_n,
  Allocator alloc_m
)
{
  verify_exception_stability (
    f,
    strong,
    gch::small_vector<T, N, Allocator> { ni.begin (), ni.end (), alloc_n },
    [&]() -> gch::small_vector<T, N, Allocator> {
      gch::small_vector<T, N, Allocator> n { ni.begin (), ni.end (), alloc_n };
      ni (n);
      return n;
    },
    [&]() -> gch::small_vector<T, M, Allocator> {
      gch::small_vector<T, M, Allocator> m { mi.begin (), mi.end (), alloc_m };
      mi (m);
      return m;
    }
  );

  verify_exception_stability (
    f,
    strong,
    gch::small_vector<T, N, Allocator> { mi.begin (), mi.end (), alloc_n },
    [&]() -> gch::small_vector<T, N, Allocator> {
      gch::small_vector<T, N, Allocator> n { mi.begin (), mi.end (), alloc_n };
      ni (n);
      return n;
    },
    [&]() -> gch::small_vector<T, M, Allocator> {
      gch::small_vector<T, M, Allocator> m { ni.begin (), ni.end (), alloc_m };
      mi (m);
      return m;
    }
  );
}

template <typename Functor, typename T, unsigned N, typename Allocator = std::allocator<T>>
void
verify_basic_exception_safety (Functor f,
                               vector_initializer<T, N, Allocator> vi,
                               Allocator alloc = Allocator ())
{
  verify_exception_stability (f, false, vi, alloc);
}

template <typename Functor,
          typename T,
          unsigned N,
          unsigned M,
          typename Allocator = std::allocator<T>>
void
verify_basic_exception_safety (Functor f,
                               vector_initializer<T, N, Allocator> ni,
                               vector_initializer<T, M, Allocator> mi,
                               Allocator alloc_n = Allocator (),
                               Allocator alloc_m = Allocator ())
{
  verify_exception_stability (f, false, ni, mi, alloc_n, alloc_m);
}

template <typename Functor, typename T, unsigned N, typename Allocator = std::allocator<T>>
inline
void
verify_strong_exception_guarantee (Functor f,
                                   vector_initializer<T, N, Allocator> vi,
                                   Allocator alloc = Allocator ())
{
  verify_exception_stability (f, true, vi, alloc);
}

template <typename Functor,
          typename T,
          unsigned N,
          unsigned M,
          typename Allocator = std::allocator<T>>
void
verify_strong_exception_guarantee (Functor f,
                                   vector_initializer<T, N, Allocator> ni,
                                   vector_initializer<T, M, Allocator> mi,
                                   Allocator alloc_n = Allocator (),
                                   Allocator alloc_m = Allocator ())
{
  verify_exception_stability (f, true, ni, mi, alloc_n, alloc_m);
}

template <template <typename, typename> class TesterT,
          template <typename ...> class AllocatorT, typename ...AArgs,
          typename std::enable_if<
                std::is_constructible<AllocatorT<int, AArgs...>, int>::value
            &&  std::is_constructible<TesterT<int, AllocatorT<int, AArgs...>>,
                                      AllocatorT<int, AArgs...>,
                                      AllocatorT<int, AArgs...>>::value
          >::type * = nullptr>
inline GCH_SMALL_VECTOR_TEST_CONSTEXPR
void
test_with_allocator (void)
{
  using namespace gch::test_types;
  TesterT<trivially_copyable_data_base, AllocatorT<trivially_copyable_data_base, AArgs...>> { } ();
  TesterT<nontrivial_data_base, AllocatorT<nontrivial_data_base, AArgs...>> { } ();

  {
    AllocatorT<trivially_copyable_data_base, AArgs...> tc_alloc_v (1);
    AllocatorT<trivially_copyable_data_base, AArgs...> tc_alloc_w (2);
    TesterT<trivially_copyable_data_base, AllocatorT<trivially_copyable_data_base, AArgs...>> {
      tc_alloc_v,
      tc_alloc_w
    } ();
  }

  {
    AllocatorT<nontrivial_data_base, AArgs...> nt_alloc_v (3);
    AllocatorT<nontrivial_data_base, AArgs...> nt_alloc_w (4);
    TesterT<nontrivial_data_base, AllocatorT<nontrivial_data_base, AArgs...>> {
      nt_alloc_v,
      nt_alloc_w
    } ();
  }

#ifdef GCH_SMALL_VECTOR_TEST_EXCEPTION_SAFETY_TESTING
  TesterT<triggering_type, AllocatorT<triggering_type, AArgs...>> { } ();
  {
    AllocatorT<triggering_type, AArgs...> trig_alloc_v (5);
    AllocatorT<triggering_type, AArgs...> trig_alloc_w (6);
    TesterT<triggering_type, AllocatorT<triggering_type, AArgs...>> {
      trig_alloc_v,
      trig_alloc_w
    } ();
  }

  TesterT<triggering_ctor, AllocatorT<triggering_ctor, AArgs...>> { } ();
  {
    AllocatorT<triggering_ctor, AArgs...> trig_alloc_v (5);
    AllocatorT<triggering_ctor, AArgs...> trig_alloc_w (6);
    TesterT<triggering_ctor, AllocatorT<triggering_ctor, AArgs...>> {
      trig_alloc_v,
      trig_alloc_w
    } ();
  }
#endif
}

template <template <typename, typename> class TesterT,
  template <typename ...> class AllocatorT, typename ...AArgs,
          typename std::enable_if<
                std::is_constructible<AllocatorT<int, AArgs...>, int>::value
            &&! std::is_constructible<TesterT<int, AllocatorT<int, AArgs...>>,
                                      AllocatorT<int, AArgs...>,
                                      AllocatorT<int, AArgs...>>::value
          >::type * = nullptr>
inline GCH_SMALL_VECTOR_TEST_CONSTEXPR
void
test_with_allocator (void)
{
  using namespace gch::test_types;
  TesterT<trivially_copyable_data_base, AllocatorT<trivially_copyable_data_base, AArgs...>> { } ();
  TesterT<nontrivial_data_base, AllocatorT<nontrivial_data_base, AArgs...>> { } ();

  AllocatorT<trivially_copyable_data_base, AArgs...> tc_alloc (1);
  TesterT<trivially_copyable_data_base, AllocatorT<trivially_copyable_data_base, AArgs...>> {
    tc_alloc
  } ();

  AllocatorT<nontrivial_data_base, AArgs...> nt_alloc (2);
  TesterT<nontrivial_data_base, AllocatorT<nontrivial_data_base, AArgs...>> {
    nt_alloc
  } ();

#ifdef GCH_SMALL_VECTOR_TEST_EXCEPTION_SAFETY_TESTING
  TesterT<triggering_type, AllocatorT<triggering_type, AArgs...>> { } ();
  {
    AllocatorT<triggering_type, AArgs...> trig_alloc (3);
    TesterT<triggering_type, AllocatorT<triggering_type, AArgs...>> {
      trig_alloc
    } ();
  }

  TesterT<triggering_ctor, AllocatorT<triggering_ctor, AArgs...>> { } ();
  {
    AllocatorT<triggering_ctor, AArgs...> trig_alloc (3);
    TesterT<triggering_ctor, AllocatorT<triggering_ctor, AArgs...>> {
      trig_alloc
    } ();
  }
#endif
}

template <template <typename, typename> class TesterT,
          template <typename ...> class AllocatorT, typename ...AArgs,
          typename std::enable_if<! std::is_constructible<AllocatorT<int, AArgs...>, int>::value
          >::type * = nullptr>
inline GCH_SMALL_VECTOR_TEST_CONSTEXPR
void
test_with_allocator (void)
{
  using namespace gch::test_types;
  TesterT<trivially_copyable_data_base, AllocatorT<trivially_copyable_data_base, AArgs...>> { } ();
  TesterT<nontrivial_data_base, AllocatorT<nontrivial_data_base, AArgs...>> { } ();

#ifdef GCH_SMALL_VECTOR_TEST_EXCEPTION_SAFETY_TESTING
  TesterT<triggering_type, AllocatorT<triggering_type, AArgs...>> { } ();
  TesterT<triggering_ctor, AllocatorT<triggering_ctor, AArgs...>> { } ();
#endif
}

template <template <typename, typename> class TesterT>
inline GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_with_allocators (void)
{
  using namespace gch::test_types;

  test_with_allocator<TesterT, std::allocator> ();
  test_with_allocator<TesterT, sized_allocator, std::uint8_t> ();
  test_with_allocator<TesterT, fancy_pointer_allocator> ();
  test_with_allocator<TesterT, allocator_with_id> ();
  test_with_allocator<TesterT, propagating_allocator_with_id> ();

#ifndef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
  test_with_allocator<TesterT, verifying_allocator_with_traits<true, true, true>::type> ();
  test_with_allocator<TesterT, verifying_allocator_with_traits<true, true, false>::type> ();
  test_with_allocator<TesterT, verifying_allocator_with_traits<true, false, true>::type> ();
  test_with_allocator<TesterT, verifying_allocator_with_traits<true, false, false>::type> ();
  test_with_allocator<TesterT, verifying_allocator_with_traits<false, true, true>::type> ();
  test_with_allocator<TesterT, verifying_allocator_with_traits<false, true, false>::type> ();
  test_with_allocator<TesterT, verifying_allocator_with_traits<false, false, true>::type> ();
  test_with_allocator<TesterT, verifying_allocator_with_traits<false, false, false>::type> ();
#endif

  return 0;
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void);

#ifdef GCH_SMALL_VECTOR_TEST_FILE
#  define QUOTED_HELPER(...) #__VA_ARGS__
#  define QUOTED(...) QUOTED_HELPER (__VA_ARGS__)
#  include QUOTED (GCH_SMALL_VECTOR_TEST_FILE)
#endif

#endif // SMALL_VECTOR_UNIT_TEST_COMMON_HPP
