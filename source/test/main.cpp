/** main.cpp
 * Main testing
 *
 * Copyright © 2021 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <type_traits>

template <bool B>
using bool_constant = std::integral_constant<bool, B>;

template <typename T, typename Enable = void>
struct underlying_if_enum
{
  using type = T;
};

template <typename T>
struct underlying_if_enum<T, typename std::enable_if<std::is_enum<T>::value>::type>
  : std::underlying_type<T>
{ };

template <typename T>
using underlying_if_enum_t = typename underlying_if_enum<T>::type;

template <typename From, typename To>
struct is_memcpyable_integral
{
  using from = underlying_if_enum_t<From>;
  using to   = underlying_if_enum_t<To>;

  static constexpr
  bool
    value = (sizeof(from) == sizeof(to))
    && (std::is_same<bool, from>::value == std::is_same<bool, to>::value)
    && std::is_integral<from>::value
    && std::is_integral<to>::value;
};

template <typename From, typename To>
struct is_convertible_pointer
  : bool_constant<std::is_pointer<From>::value
              &&  std::is_pointer<To>::value
              &&  std::is_convertible<From, To>::value>
{ };

// memcpyable assignment
template <typename QualifiedFrom, typename QualifiedTo>
struct is_memcpyable
{
  static_assert (! std::is_reference<QualifiedTo>::value, "qualifiedto");

  using from = typename std::remove_reference<typename std::remove_cv<QualifiedFrom>::type>::type;
  using to   = typename std::remove_cv<QualifiedTo>::type;

  static constexpr
  bool
    value = std::is_trivially_assignable<QualifiedTo&, QualifiedFrom>::value
    &&  std::is_trivially_copyable<to>::value
    &&  (  std::is_same<typename std::remove_cv<from>::type, to>::value
       ||  is_memcpyable_integral<from, to>::value
       ||  is_convertible_pointer<from, to>::value);
};

static_assert (  is_memcpyable<               int, int>::value, "memcpyable");
static_assert (  is_memcpyable<const          int, int>::value, "memcpyable");
static_assert (  is_memcpyable<      volatile int, int>::value, "memcpyable");
static_assert (  is_memcpyable<const volatile int, int>::value, "memcpyable");

static_assert (! is_memcpyable<               int, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const          int, const int>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile int, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int, const int>::value, "memcpyable");

static_assert (  is_memcpyable<               int, volatile int>::value, "memcpyable");
static_assert (  is_memcpyable<const          int, volatile int>::value, "memcpyable");
static_assert (  is_memcpyable<      volatile int, volatile int>::value, "memcpyable");
static_assert (  is_memcpyable<const volatile int, volatile int>::value, "memcpyable");

static_assert (! is_memcpyable<               int, const volatile int>::value, "memcpyable");
static_assert (! is_memcpyable<const          int, const volatile int>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile int, const volatile int>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int, const volatile int>::value, "memcpyable");

static_assert (is_memcpyable<               int&, int>::value, "memcpyable");
static_assert (is_memcpyable<const          int&, int>::value, "memcpyable");
static_assert (is_memcpyable<      volatile int&, int>::value, "memcpyable");
static_assert (is_memcpyable<const volatile int&, int>::value, "memcpyable");

static_assert (! is_memcpyable<               int&, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const          int&, const int>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile int&, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int&, const int>::value, "memcpyable");

static_assert (is_memcpyable<               int&&, int>::value, "memcpyable");
static_assert (is_memcpyable<const          int&&, int>::value, "memcpyable");
static_assert (is_memcpyable<      volatile int&&, int>::value, "memcpyable");
static_assert (is_memcpyable<const volatile int&&, int>::value, "memcpyable");

static_assert (! is_memcpyable<               int&&, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const          int&&, const int>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile int&&, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int&&, const int>::value, "memcpyable");

static_assert (  is_memcpyable<               int *, int *>::value, "memcpyable");
static_assert (! is_memcpyable<const          int *, int *>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile int *, int *>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int *, int *>::value, "memcpyable");

static_assert (  is_memcpyable<               int *, const int *>::value, "memcpyable");
static_assert (  is_memcpyable<const          int *, const int *>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile int *, const int *>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int *, const int *>::value, "memcpyable");

static_assert (  is_memcpyable<               int *, volatile int *>::value, "memcpyable");
static_assert (! is_memcpyable<const          int *, volatile int *>::value, "memcpyable");
static_assert (  is_memcpyable<      volatile int *, volatile int *>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int *, volatile int *>::value, "memcpyable");

static_assert (  is_memcpyable<               int *, const volatile int *>::value, "memcpyable");
static_assert (  is_memcpyable<const          int *, const volatile int *>::value, "memcpyable");
static_assert (  is_memcpyable<      volatile int *, const volatile int *>::value, "memcpyable");
static_assert (  is_memcpyable<const volatile int *, const volatile int *>::value, "memcpyable");

static_assert (  is_memcpyable<               int * const, int *>::value, "memcpyable");
static_assert (! is_memcpyable<const          int * const, int *>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile int * const, int *>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile int * const, int *>::value, "memcpyable");

static_assert (  is_memcpyable<               int * const, const volatile int *>::value, "memcpyable");
static_assert (  is_memcpyable<const          int * const, const volatile int *>::value, "memcpyable");
static_assert (  is_memcpyable<      volatile int * const, const volatile int *>::value, "memcpyable");
static_assert (  is_memcpyable<const volatile int * const, const volatile int *>::value, "memcpyable");

enum myenum : int
{
  x = 1
};

static_assert (  is_memcpyable<               myenum, int>::value, "memcpyable");
static_assert (  is_memcpyable<const          myenum, int>::value, "memcpyable");
static_assert (  is_memcpyable<      volatile myenum, int>::value, "memcpyable");
static_assert (  is_memcpyable<const volatile myenum, int>::value, "memcpyable");

static_assert (! is_memcpyable<               myenum, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const          myenum, const int>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile myenum, const int>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile myenum, const int>::value, "memcpyable");

static_assert (  is_memcpyable<               myenum, volatile int>::value, "memcpyable");
static_assert (  is_memcpyable<const          myenum, volatile int>::value, "memcpyable");
static_assert (  is_memcpyable<      volatile myenum, volatile int>::value, "memcpyable");
static_assert (  is_memcpyable<const volatile myenum, volatile int>::value, "memcpyable");

static_assert (! is_memcpyable<               myenum, const volatile int>::value, "memcpyable");
static_assert (! is_memcpyable<const          myenum, const volatile int>::value, "memcpyable");
static_assert (! is_memcpyable<      volatile myenum, const volatile int>::value, "memcpyable");
static_assert (! is_memcpyable<const volatile myenum, const volatile int>::value, "memcpyable");

#include "gch/small_vector.hpp"

#include "test-types.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#ifdef GCH_LIB_CONCEPTS
namespace gch
{
  namespace concepts
  {
    template <typename X, typename T>
    concept Container =
          DefaultConstructible<X>
      &&  CopyConstructible<X>
      &&  EqualityComparable<X>
      &&  Swappable<X>
      &&  CopyInsertable<T, X>
      &&  EqualityComparable<T>
      &&  Destructible<T>
      &&  requires (X a, X b, X&& rv)
          {
            // refer to [tab:container.req]

            // X::value_type
            typename X::value_type;
            requires std::same_as<typename X::value_type, T>;
            requires Erasable<T, X>; // precondition, not a requirement, but UB if not met

            // X::reference
            typename X::reference;
            requires std::same_as<typename X::reference, T&>;

            // X::const_reference
            typename X::const_reference;
            requires std::same_as<typename X::const_reference, const T&>;

            // X::iterator
            typename X::iterator;
            requires std::forward_iterator<typename X::iterator>;

            // X::const_iterator
            typename X::const_iterator;
            requires std::forward_iterator<typename X::iterator>;
            requires std::convertible_to<typename X::iterator, typename X::const_iterator>;

            // X::difference_type
            typename X::difference_type;
            requires std::signed_integral<typename X::difference_type>;

            requires std::same_as<
              typename X::difference_type,
              typename std::iterator_traits<typename X::iterator>::difference_type>;

            requires std::same_as<
              typename X::difference_type,
              typename std::iterator_traits<typename X::const_iterator>::difference_type>;

            // X::size_type
            typename X::size_type;
            requires std::unsigned_integral<typename X::size_type>;

            requires
              (
                (std::numeric_limits<typename X::difference_type>::max) ()
                <=
                (std::numeric_limits<typename X::size_type>::max) ()
              );

            { X ()               } -> std::same_as<X>;
            { X (a)              } -> std::same_as<X>;
            { X (std::move (rv)) } -> std::same_as<X>;
            { a = b              } -> std::same_as<X&>;
            { a = std::move (rv) } -> std::same_as<X&>;
            { a.~X ()            } -> std::same_as<void>;
            { a.begin ()         } -> std::same_as<typename X::iterator>;
            { a.end ()           } -> std::same_as<typename X::iterator>;
            { a.cbegin ()        } -> std::same_as<typename X::const_iterator>;
            { a.cend ()          } -> std::same_as<typename X::const_iterator>;
            { a == b             } -> std::convertible_to<bool>;
            { a != b             } -> std::convertible_to<bool>;
            { a.swap (b)         } -> std::same_as<void>;
            { a.size ()          } -> std::same_as<typename X::size_type>;
            { a.max_size ()      } -> std::same_as<typename X::size_type>;
            { a.empty ()         } -> std::convertible_to<bool>;

            { const_cast<const X&> (a).begin () } -> std::same_as<typename X::const_iterator>;
            { const_cast<const X&> (a).end ()   } -> std::same_as<typename X::const_iterator>;
          };

    template <typename X, typename T>
    concept ReversibleContainer =
          Container<X, T>
      &&  requires (X a)
          {
            // X::reverse_iterator
            typename X::reverse_iterator;
            typename std::reverse_iterator<typename X::iterator>;
            requires std::same_as<typename X::reverse_iterator::value_type, T>;

            // X::const_reverse_iterator
            typename X::const_reverse_iterator;
            typename std::reverse_iterator<typename X::const_iterator>;
            requires std::same_as<typename X::const_reverse_iterator::value_type, T>;

            { a.rbegin ()  } -> std::same_as<typename X::reverse_iterator>;
            { a.rend ()    } -> std::same_as<typename X::reverse_iterator>;
            { a.crbegin () } -> std::same_as<typename X::const_reverse_iterator>;
            { a.crend ()   } -> std::same_as<typename X::const_reverse_iterator>;

            { const_cast<const X&> (a).rbegin () }
              -> std::same_as<typename X::const_reverse_iterator>;

            { const_cast<const X&> (a).rend () }
              -> std::same_as<typename X::const_reverse_iterator>;

            { std::reverse_iterator (a.end ())   };
            { std::reverse_iterator (a.begin ()) };

            { std::reverse_iterator (const_cast<const X&> (a).end ())   };
            { std::reverse_iterator (const_cast<const X&> (a).begin ()) };
          };

    template <typename X, typename T, typename A>
    concept AllocatorAwareContainer =
          CopyAssignable<X>
      &&  MoveInsertable<T, X, A>
      &&  CopyInsertable<T, X, A>
      &&  DefaultConstructible<A>
      &&  requires (X a, X b, X& t, X&& rv, A m)
          {
            // refer to [tab:container.alloc.req]

            typename X::allocator_type;
            requires std::same_as<typename X::allocator_type, A>;
            requires std::same_as<typename X::allocator_type::value_type, typename X::value_type>;

            { a.get_allocator ()            } -> std::same_as<A>;
            { X (m)                         };
            { X (t, m)                      };
            { X (std::move (rv))            };
            { X (std::move (rv), m)         };
            { a = t                         } -> std::same_as<X&>;
            { a = const_cast<const X&>  (t) } -> std::same_as<X&>;
            { a = const_cast<const X&&> (t) } -> std::same_as<X&>;
            { a = std::move (rv)            } -> std::same_as<X&>;
            { a.swap (b)                    } -> std::same_as<void>;

            requires std::allocator_traits<A>::propagate_on_container_move_assignment
                 ||  (MoveAssignable<T> && MoveInsertable<T, X, A>);
          };

    // template <typename X, typename T,
    //           typename A = std::conditional<requires { typename X::allocator_type; },
    //                                         typename X::allocator_type,
    //                                         std::allocator<T>>>
    // concept SequenceContainer =
    //       Container<X, T>
    //   &&  MoveInsertable<T>
    //   &&  CopyAssignable<T>
    //   &&  CopyInsertable<T, X>
    //   &&  requires (X a,
    //                 typename X::const_iterator p,
    //                 typename X::const_iterator q,
    //                 typename X::const_iterator q1,
    //                 typename X::const_iterator q2,
    //                 std::initializer_list<typename X::value_type> il,
    //                 typename X::size_type n,
    //                 typename X::value_type& t,
    //                 typename x::value_type&& rv)
    //       {
    //         // [tab:container.seq.req]
    //         { X (n, t) };
    //         {  }
    //       };
  }
}
#endif

template <typename T>
struct test1
{
  using type = T;
};

template <typename U>
struct test2
{
  using t1 = test1<U>;
};

template <typename T2>
using test_u = typename T2::t1::type;

test_u<test2<int>> xx = 2;

using namespace gch;

template class gch::small_vector<int>;
template class gch::small_vector<std::size_t>;
template class gch::small_vector<double>;
template class gch::small_vector<char *>;

// static_assert (gch::concepts::NullablePointer<gch::test_types::pointer_wrapper<double>>);
template class gch::small_vector<double, 8, gch::test_types::weird_allocator<double>>;
// template class gch::small_vector<double, 8, gch::test_types::weird_allocator2<double>>;

#ifdef GCH_LIB_CONCEPTS
static_assert (concepts::NullablePointer<test_types::pointer_wrapper<int>>
           &&  std::random_access_iterator<test_types::pointer_wrapper<int>>
           &&  std::contiguous_iterator<test_types::pointer_wrapper<int>>);
#endif

// static_assert (! gch::concepts::MoveInsertable<gch::test_types::uncopyable, gch::small_vector<gch::test_types::uncopyable>, std::allocator<gch::test_types::uncopyable>>);

using namespace gch;

// struct X {
//   X(const X&) noexcept (false) = default;
// };
//
// static_assert(std::is_trivially_copyable_v<X>, "");
// static_assert(std::is_copy_constructible_v<X>, "");
// static_assert(!std::is_nothrow_copy_constructible_v<X>, "");
// static_assert(!std::is_trivially_copy_constructible_v<X>, "");

template <typename T> struct my_allocator : std::allocator<T> { };

namespace std
{
  template <typename T>
  struct allocator_traits<my_allocator<T>> : allocator_traits<allocator<T>>
  {
    using size_type = std::uint16_t;

    template <typename U>
    using rebind_alloc = my_allocator<U>;
  };

  template <>
  struct allocator_traits<my_allocator<double>> : allocator_traits<allocator<double>>
  {
    using size_type = std::uint8_t;

    template <typename U>
    using rebind_alloc = my_allocator<U>;
  };
}

#ifdef GCH_CONSTEXPR_SMALL_VECTOR
constexpr int test_func0 (void)
{
  small_vector<int> c { };
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func1 (void)
{
  small_vector<int> c (3, 2);
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func2 (void)
{
  small_vector<int> c (2);
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func3 (void)
{
  std::array<int, 3> a { 2, 4, 6 };
  small_vector<int> c (a.begin (), a.end ());
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func4 (void)
{
  small_vector<int> c { 1, 2, 3 };
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func5 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func6 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  c.emplace (c.begin () + 1, 7);
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func7 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  small_vector<test_types::non_trivial> d { 7, 8, 9 };
  c = d;
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr int test_func8 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  small_vector<test_types::non_trivial> d { 7, 8, 9 };
  c = std::move (d);
  return std::accumulate (c.begin (), c.end (), 0);
}
#endif

int main (void)
{
  small_vector<double, default_buffer_size<my_allocator<double>>::value, my_allocator<double>> x;

#ifdef GCH_CONSTEXPR_SMALL_VECTOR
  constexpr std::array a {
    test_func0 (),
    test_func1 (),
    test_func2 (),
    test_func3 (),
    test_func4 (),
    test_func5 (),
    test_func6 (),
    test_func7 (),
    test_func8 (),
  };

  std::array b {
    test_func0 (),
    test_func1 (),
    test_func2 (),
    test_func3 (),
    test_func4 (),
    test_func5 (),
    test_func6 (),
    test_func7 (),
    test_func8 (),
  };

  std::cout << "a == b? " << std::equal (a.begin (), a.end (),
                                         b.begin (), b.end ()) << std::endl << std::endl;

#endif


  small_vector<int> v { 1, 2, 3 };
  v.insert (v.begin () + 1, 7);
  for (auto e : v)
    std::cout << e << std::endl;

  small_vector<test_types::uncopyable> u { };
  small_vector<test_types::uncopyable> uu { };
  // uu.emplace_back (2);

  std::vector<test_types::uncopyable> vv { };
  // vv.emplace_back (2);

  x.push_back (17);
  x.push_back (22);
  for (auto e : x)
    std::cout << e << std::endl;

  std::cout << sizeof (x) << std::endl;

  static_assert (std::numeric_limits<decltype(x)::difference_type>::max () <=
                 std::numeric_limits<decltype(x)::size_type>::max (), "");

  std::cout << "1:" << sizeof (small_vector<std::uint16_t, 6, my_allocator<std::uint16_t>>) << std::endl;
  std::cout << "2:" << sizeof (small_vector<std::uint16_t, 0, my_allocator<std::uint16_t>>) << std::endl;

  std::cout << static_cast<std::size_t> (x.max_size ()) << std::endl;
  std::cout << sizeof (small_vector<double, 0, my_allocator<double>>) << std::endl << std::endl;

  std::cout << default_buffer_size<my_allocator<double>>::value << std::endl;
  std::cout << alignof (detail::inline_storage<std::uint16_t, 6>) << std::endl;
  std::cout << alignof (detail::inline_storage<std::uint16_t, 0>) << std::endl;
  std::cout << sizeof (small_vector<std::uint16_t, 0, my_allocator<std::uint16_t>>) << std::endl;
  std::cout << sizeof (gch::detail::small_vector_base<my_allocator<std::uint16_t>, 6>) << std::endl;

  std::cout << sizeof (small_vector<double, 0, std::allocator<double>>) << std::endl;

  try
  {
    x.assign (128, 0.3);
  }
  catch (...)
  {
    std::cout << "successfully caught" << std::endl;
  }

  return 0;
}
