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

template class gch::small_vector<int>;
template class gch::small_vector<std::size_t>;
template class gch::small_vector<double>;
template class gch::small_vector<char *>;

// static_assert (gch::concepts::NullablePointer<gch::test_types::pointer_wrapper1<double>>);
template class gch::small_vector<double, 8, gch::test_types::weird_allocator1<double>>;
// template class gch::small_vector<double, 8, gch::test_types::weird_allocator2<double>>;

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

int main (void)
{
  using namespace gch;

  constexpr int x = 4;
  constexpr int y = 2;
  // constexpr small_vector<int> c (x, y);
  // constexpr std::vector<int> cv (4, 2);

  small_vector<int> v { 1, 2, 3 };
  v.insert (v.begin () + 1, 7);
  for (auto e : v)
    std::cout << e << std::endl;

  small_vector<test_types::uncopyable> u { };
  small_vector<test_types::uncopyable> uu { };
  // uu.emplace_back (2);

  std::vector<test_types::uncopyable> vv { };
  // vv.emplace_back (2);



  return 0;
}
