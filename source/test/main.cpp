/** main.cpp
 * Main testing
 *
 * Copyright © 2021 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <utility>

// testing for ambiguity in comparison operators
using namespace std::rel_ops;

#include "gch/small_vector.hpp"

#include "test-types.hpp"

#include <type_traits>

#include <array>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#ifdef GCH_CONCEPTS
#  include <iterator>
#endif

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

#ifdef GCH_LIB_CONCEPTS

namespace gch
{

  namespace concepts
  {

    template <typename X, typename T>
    concept Container =
          MoveConstructible<X>
      &&  Swappable<X>
      &&  requires (X a, X b, X r, X&& rv)
          {
            // refer to [tab:container.req]

            // X::value_type
            // Precondition: Erasable
            requires ! Erasable<T, X>
                   ||  requires
                       {
                         typename X::value_type;
                         requires std::same_as<typename X::value_type, T>;
                       };

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

            // X u;
            { ::new (static_cast<void *> (nullptr)) T };

            // X ()
            { X () };

            // X (a), X u (a);, X u = a;
            // Precondition: CopyInsertable
            requires ! CopyInsertable<T, X>
                   ||  (requires { { X (a) }; } && CopyConstructible<X>);

            { a = std::move (rv) } -> std::same_as<X&>;
            { a.~X ()            } -> std::same_as<void>;

            // a.begin ()
            { a.begin ()                        } -> std::same_as<typename X::iterator>;
            { const_cast<const X&> (a).begin () } -> std::same_as<typename X::const_iterator>;

            // a.end ()
            { a.end ()                          } -> std::same_as<typename X::iterator>;
            { const_cast<const X&> (a).end ()   } -> std::same_as<typename X::const_iterator>;

#ifdef GCH_LIB_THREE_WAY_COMPARISON
            requires ! std::random_access_iterator<typename X::iterator>
                   ||  requires (typename X::iterator i, typename X::iterator j)
                       {
                         { i <=> j } -> std::same_as<std::strong_ordering>;
                       };
#endif

            // a == b
            // Precondition: EqualityComparable
            requires ! EqualityComparable<T>
                   ||  requires
                       {
                         { a == b } -> std::convertible_to<bool>;
                         { a != b } -> std::convertible_to<bool>;
                       };

            { a.cbegin ()   } -> std::same_as<typename X::const_iterator>;
            { a.cend ()     } -> std::same_as<typename X::const_iterator>;
            { a.swap (b)    } -> std::same_as<void>;
            { r = a         } -> std::same_as<X&>;
            { a.size ()     } -> std::same_as<typename X::size_type>;
            { a.max_size () } -> std::same_as<typename X::size_type>;
            { a.empty ()    } -> std::convertible_to<bool>;
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

    template <typename X, typename T>
    concept AllocatorAwareContainer =
          requires { typename X::allocator_type; }
      &&  std::same_as<typename X::allocator_type::value_type, typename X::value_type>
      &&  requires (X a, X b, X& t, X&& rv, typename X::allocator_type m)
          {
            // refer to [tab:container.alloc.req]

            { a.get_allocator () } -> std::same_as<typename X::allocator_type>;

            // X(), X u;
            // Precondition: DefaultConstructible
            requires ! DefaultConstructible<typename X::allocator_type>
                   ||  requires
                       {
                         { X () };                                    // X ()
                         { ::new (static_cast<void *> (nullptr)) T }; // X u;
                       };

            // Is prvalue initialization covered by ConstructibleFrom?
            // X (m), X u (m);
            { X (m) };                                                 // X (m)
            requires ConstructibleFrom<X, typename X::allocator_type>; // X u (m)

            // X (t, m), X u (t, m);
            // Precondition: CopyInsertable
            requires ! CopyInsertable<T, X>
                   ||  requires
                       {
                         // X (t, m)
                         { X (t, m)                         };
                         { X (const_cast<const X& > (t), m) };
                         { X (const_cast<const X&&> (t), m) };

                         // X u (t, m)
                         requires ConstructibleFrom<X,       X&, typename X::allocator_type>;
                         requires ConstructibleFrom<X, const X&, typename X::allocator_type>;
                         requires ConstructibleFrom<X, const X , typename X::allocator_type>;
                       };


            { X (std::move (rv)) };           // X (rv)
            requires ConstructibleFrom<X, X>; // X u (rv)

            // X (rv, m), X u (rv, m);
            // Precondition: MoveInsertable
            requires ! MoveInsertable<T, X>
                   ||  requires
                       {
                         // X (rv, m)
                         { X (std::move (rv), m) };

                         // X u (rv, m)
                         requires ConstructibleFrom<X, X, typename X::allocator_type>;
                       };

            // a = t
            // Precondition: CopyInsertable && CopyAssignable
            requires ! (CopyInsertable<T, X> && CopyAssignable<T>)
                   ||  requires { { a = t } -> std::same_as<X&>; };

            // a = rv
            // Precondition:
            //   std::allocator_traits<A>::propagate_on_container_move_assignment == false
            //   implies MoveAssignable && MoveInsertable
            requires ! (  std::allocator_traits<typename X::allocator_type>::
                            propagate_on_container_move_assignment::value
                      ||  (MoveAssignable<T> && MoveInsertable<T, X>))
                   ||  requires { { a = std::move (rv) } -> std::same_as<X&>; };

            { a.swap (b) } -> std::same_as<void>;

          };

    namespace detail
    {

      template <typename X, typename T,
                typename A = typename std::conditional<requires { typename X::allocator_type; },
                                                       typename X::allocator_type,
                                                       std::allocator<T>>::type,
                typename InputIt = typename std::conditional<requires { typename X::iterator; },
                                                             typename X::iterator,
                                                             void>::type>
      concept SequenceContainerBase =
            Container<X, T>
        &&  requires (X a,
                      InputIt i,
                      InputIt j,
                      std::initializer_list<typename X::value_type> il,
                      typename X::size_type      n,
                      typename X::const_iterator p,
                      typename X::const_iterator q,
                      typename X::const_iterator q1,
                      typename X::const_iterator q2,
                      typename X::value_type&    t,
                      typename X::value_type&&   rv)
            {
              // [tab:container.seq.req]

              // X (n, t), X u (n, t);
              // Precondition: CopyInsertable
              requires ! CopyInsertable<T, X>
                     ||  requires
                         {
                           // X (n, t)
                           { X (n, t)                         };
                           { X (n, const_cast<const T& > (t)) };
                           { X (n, const_cast<const T&&> (t)) };

                           // X u (n, t);
                           requires ConstructibleFrom<X, decltype (n),       T&>;
                           requires ConstructibleFrom<X, decltype (n), const T&>;
                           requires ConstructibleFrom<X, decltype (n), const T>;
                         };

              // X (il)
              // Precondition: EmplaceConstructible<decltype (*std::begin (il))>
              requires ! EmplaceConstructible<T, X, A, decltype (*std::begin (il))>
                     ||  requires { { X (il) }; };

              // a = il
              // Precondition: CopyInsertable && CopyAssignable
              requires ! (CopyInsertable<T, X> && CopyAssignable<T>)
                     ||  requires { { a = il } -> std::same_as<X&>; };

              // a.insert (p, n, t)
              // Precondition: CopyInsertable && CopyAssignable
              requires ! (CopyInsertable<T, X> && CopyAssignable<T>)
                     ||  requires
                         {
                           { a.insert (p, n, t) } -> std::same_as<typename X::iterator>;

                           { a.insert (p, n, const_cast<const T& > (t)) }
                             -> std::same_as<typename X::iterator>;

                           { a.insert (p, n, const_cast<const T&&> (t)) }
                             -> std::same_as<typename X::iterator>;
                         };

              // a.clear ()
              { a.clear () } -> std::same_as<void>;

              // a.assign (il)
              // Precondition: EmplaceConstructible<decltype (*std::begin (il))>
              requires ! EmplaceConstructible<T, X, A, decltype (*std::begin (il))>
                     ||  requires { { a.assign (il) } -> std::same_as<void>; };

              // a.assign (n, t)
              // Precondition: CopyInsertable && CopyAssignable
              requires ! (CopyInsertable<T, X> && CopyAssignable<T>)
                     ||  requires
                         {
                           { a.assign (n, t)                         } -> std::same_as<void>;
                           { a.assign (n, const_cast<const T& > (t)) } -> std::same_as<void>;
                           { a.assign (n, const_cast<const T&&> (t)) } -> std::same_as<void>;
                         };

              // optional requirements [tab:container.seq.opt]

              // a.front ()
              requires ! requires { a.front (); }
                     ||  requires
                         {
                           { a.front () } -> std::same_as<typename X::reference>;

                           { const_cast<const X&> (a).front () }
                             -> std::same_as<typename X::const_reference>;
                         };

              // a.back ()
              requires ! requires { a.back (); }
                     ||  requires
                         {
                           { a.back () } -> std::same_as<typename X::reference>;

                           { const_cast<const X&> (a).back () }
                             -> std::same_as<typename X::const_reference>;
                         };

              // a.push_front (t)
              // Precondition: CopyInsertable
              requires ! requires { a.push_front (t); }
                     ||  (! CopyInsertable<T, X, A>
                        ||  requires { { a.push_front (t) } -> std::same_as<void>; });

              requires ! requires { a.push_front (const_cast<const T&> (t)); }
                     ||  (! CopyInsertable<T, X, A>
                        ||  requires
                            {
                              { a.push_front (const_cast<const T&> (t)) } -> std::same_as<void>;
                            });

              requires ! requires { a.push_front (const_cast<const T&&> (t)); }
                     ||  (! CopyInsertable<T, X, A>
                        ||  requires
                            {
                              { a.push_front (const_cast<const T&&> (t)) } -> std::same_as<void>;
                            });

              // a.push_front (rv)
              // Precondition: MoveInsertable
              requires ! requires { a.push_front (std::move (rv)); }
                     ||  (! MoveInsertable<T, X, A>
                        ||  requires { { a.push_front (std::move (rv)) } -> std::same_as<void>; });

              // a.push_back (t)
              // Precondition: CopyInsertable
              requires ! requires { a.push_back (t); }
                     ||  (! CopyInsertable<T, X, A>
                        ||  requires { { a.push_back (t) } -> std::same_as<void>; });

              requires ! requires { a.push_back (const_cast<const T&> (t)); }
                     ||  (! CopyInsertable<T, X, A>
                        ||  requires
                            {
                              { a.push_back (const_cast<const T&> (t)) } -> std::same_as<void>;
                            });

              requires ! requires { a.push_back (const_cast<const T&&> (t)); }
                     ||  (! CopyInsertable<T, X, A>
                        ||  requires
                            {
                              { a.push_back (const_cast<const T&&> (t)) } -> std::same_as<void>;
                            });

              // a.push_back (rv)
              // Precondition: MoveInsertable
              requires ! requires { a.push_back (std::move (rv)); }
                     ||  (! MoveInsertable<T, X, A>
                        ||  requires { { a.push_back (std::move (rv)) } -> std::same_as<void>; });

              // a.pop_back ()
              requires ! requires { a.pop_back (); }
                     ||  requires { { a.pop_back () } -> std::same_as<void>; };

              // a[n]
              requires ! requires { a[n]; }
                     ||  requires
                         {
                           { a[n] } -> std::same_as<typename X::reference>;
                           { const_cast<const X&> (a)[n] }
                             -> std::same_as<typename X::const_reference>;
                         };

              // a.at (n)
              requires ! requires { a.at (n); }
                     ||  requires
                         {
                           { a.at (n) } -> std::same_as<typename X::reference>;
                           { const_cast<const X&> (a).at (n) }
                             -> std::same_as<typename X::const_reference>;
                         };
            };

    }

    template <typename X, typename T,
              typename A = typename std::conditional<requires { typename X::allocator_type; },
                                                     typename X::allocator_type,
                                                     std::allocator<T>>::type,
              typename InputIt = typename std::conditional<requires { typename X::iterator; },
                                                           typename X::iterator,
                                                           void>::type>
    concept SequenceContainer =
          detail::SequenceContainerBase<X, T, A, InputIt>
      &&  requires (X a,
                    InputIt i,
                    InputIt j,
                    std::initializer_list<typename X::value_type> il,
                    typename X::size_type      n,
                    typename X::const_iterator p,
                    typename X::const_iterator q,
                    typename X::const_iterator q1,
                    typename X::const_iterator q2,
                    typename X::value_type&    t,
                    typename X::value_type&&   rv)
          {
            // Basic case of where we don't meet vector or deque requirements

            // [tab:container.seq.req]

            // X (i, j), X u (i, j);
            // Precondition: EmplaceConstructible<decltype (*i)>
            requires ! EmplaceConstructible<T, X, A, decltype (*i)>
                   ||  requires
                       {
                         // X (i, j)
                         { X (i, j) };

                         // X u (i, j);
                         requires ConstructibleFrom<X, InputIt, InputIt>;
                       };

            // a.emplace (p, args)
            // Precondition: EmplaceConstructible<Args...>
            // can't do anything here without Args...

            // a.insert (p, t)
            // Precondition: CopyInsertable
            requires ! CopyInsertable<T, X>
                   ||  requires
                       {
                         { a.insert (p, t) } -> std::same_as<typename X::iterator>;

                         { a.insert (p, const_cast<const T& > (t)) }
                           -> std::same_as<typename X::iterator>;

                         { a.insert (p, const_cast<const T&&> (t)) }
                           -> std::same_as<typename X::iterator>;
                       };

            // a.insert (p, rv)
            // Precondition: MoveInsertable
            requires ! MoveInsertable<T, X>
                   ||  requires
                       {
                         { a.insert (p, std::move (rv)) } -> std::same_as<typename X::iterator>;
                       };

            // a.insert (p, i, j)
            // Precondition: EmplaceConstructible<decltype (*i)>
            requires ! EmplaceConstructible<T, X, A, decltype (*i)>
                   ||  requires { { a.insert (p, i, j) } -> std::same_as<typename X::iterator>; };

            // a.insert (p, il)
            // Precondition: same as above
            requires ! EmplaceConstructible<T, X, A, decltype (*il.begin ())>
                   ||  requires { { a.insert (p, il) } -> std::same_as<typename X::iterator>; };

            // a.erase (q)
            { a.erase (q)    } -> std::same_as<typename X::iterator>;

            // a.erase (p, q)
            { a.erase (p, q) } -> std::same_as<typename X::iterator>;

            // a.assign (i, j)
            // Precondition: EmplaceConstructible<decltype (*i)>
            requires ! EmplaceConstructible<T, X, A, decltype (*i)>
                   ||  requires { { a.assign (i, j) } -> std::same_as<void>; };

            // optional requirements [tab:container.seq.opt]

            // a.emplace_front (args), a.emplace_back (args)
            // Precondition: EmplaceConstructible<Args...>
            // can't do anything here without Args...
          };

    template <typename X, typename T,
              typename A = typename std::conditional<requires { typename X::allocator_type; },
                                                     typename X::allocator_type,
                                                     std::allocator<T>>::type,
              typename InputIt = typename std::conditional<requires { typename X::iterator; },
                                                           typename X::iterator,
                                                           void>::type>
    concept VectorSequenceContainer =
          detail::SequenceContainerBase<X, T, A, InputIt>
      &&  requires (X a,
                    InputIt i,
                    InputIt j,
                    std::initializer_list<typename X::value_type> il,
                    typename X::size_type      n,
                    typename X::const_iterator p,
                    typename X::const_iterator q,
                    typename X::const_iterator q1,
                    typename X::const_iterator q2,
                    typename X::value_type&    t,
                    typename X::value_type&&   rv)
          {
            // Basic case of where we don't meet vector or deque requirements

            // [tab:container.seq.req]

            // X (i, j), X u (i, j);
            // Precondition: MoveInsertable && EmplaceConstructible<decltype (*i)>
            requires ! (  EmplaceConstructible<T, X, A, decltype (*i)>
                      &&  (std::forward_iterator<InputIt> || MoveInsertable<T, X>))
                   ||  requires
                       {
                         // X (i, j)
                         { X (i, j) };

                         // X u (i, j);
                         requires ConstructibleFrom<X, InputIt, InputIt>;
                       };

            // a.emplace (p, args)
            // Precondition: EmplaceConstructible<Args...>
            // can't do anything here without Args...

            // a.insert (p, t)
            // Precondition: CopyInsertable
            requires ! (CopyInsertable<T, X> && CopyAssignable<T>)
                   ||  requires
                       {
                         { a.insert (p, t) } -> std::same_as<typename X::iterator>;

                         { a.insert (p, const_cast<const T& > (t)) }
                           -> std::same_as<typename X::iterator>;

                         { a.insert (p, const_cast<const T&&> (t)) }
                           -> std::same_as<typename X::iterator>;
                       };

            // a.insert (p, rv)
            // Precondition: MoveInsertable
            requires ! (MoveInsertable<T, X> && MoveAssignable<T>)
                   ||  requires
                       {
                         { a.insert (p, std::move (rv)) } -> std::same_as<typename X::iterator>;
                       };

            // a.insert (p, i, j)
            // Precondition: EmplaceConstructible<decltype (*i)>
            requires ! (  EmplaceConstructible<T, X, A, decltype (*i)>
                      &&  MoveInsertable<T, X>
                      &&  MoveConstructible<T>
                      &&  MoveAssignable<T>
                      &&  Swappable<T>)
                   ||  requires { { a.insert (p, i, j) } -> std::same_as<typename X::iterator>; };

            // a.insert (p, il)
            // Precondition: same as above
            requires ! (  EmplaceConstructible<T, X, A, decltype (*i)>
                      &&  MoveInsertable<T, X>
                      &&  MoveConstructible<T>
                      &&  MoveAssignable<T>
                      &&  Swappable<T>)
                   ||  requires { { a.insert (p, il) } -> std::same_as<typename X::iterator>; };

            // a.erase (q)
            // Precondition: MoveAssignable
            requires ! MoveAssignable<T>
                   ||  requires { { a.erase (q) } -> std::same_as<typename X::iterator>; };

            // a.erase (p, q)
            // Precondition: MoveAssignable
            requires ! MoveAssignable<T>
                   ||  requires { { a.erase (p, q) } -> std::same_as<typename X::iterator>; };

            // a.assign (i, j)
            // Precondition: EmplaceConstructible<decltype (*i)>
            requires ! (  EmplaceConstructible<T, X, A, decltype (*i)>
                      &&  (std::forward_iterator<InputIt> || MoveInsertable<T, X>))
                   ||  requires { { a.assign (i, j) } -> std::same_as<void>; };

            // optional requirements [tab:container.seq.opt]

            // a.emplace_front (args), a.emplace_back (args)
            // Precondition: EmplaceConstructible<Args...>
            // can't do anything here without Args...
          };

    template <typename X, typename T>
    concept ContiguousContainer =
          Container<X, T>
      &&  std::random_access_iterator<typename X::iterator>       // FIXME: technically shouldn't
      &&  std::random_access_iterator<typename X::const_iterator> //        be using the concept
      &&  std::contiguous_iterator<typename X::iterator>
      &&  std::contiguous_iterator<typename X::const_iterator>;

    template <typename X,
              typename T = typename std::conditional<requires { typename X::value_type; },
                                                     typename X::value_type,
                                                     void>::type>
    concept Vector = ! std::same_as<T, void>         // <- ie. disallow the default if
                   &&  Container<              X, T> //        X::value_type is not defined
                   &&  AllocatorAwareContainer<X, T>
                   &&  VectorSequenceContainer<X, T>
                   &&  ContiguousContainer<    X, T>
                   &&  ReversibleContainer<    X, T>;

    static_assert (Vector<std::vector<double>>);
    static_assert (Vector<gch::small_vector<double>>);
    static_assert (! Vector<std::list<double>>);

  }

}

#endif

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

template <typename T>
struct tiny_allocator
  : std::allocator<T>
{
  using size_type = std::uint16_t;

  void
  max_size (void) = delete;
};

template <>
struct tiny_allocator<double>
  : std::allocator<double>
{
  using size_type = std::uint8_t;

  void
  max_size (void) = delete;
};

template <typename T>
constexpr
bool
operator!= (const tiny_allocator<T>&, const tiny_allocator<T>&) noexcept
{
  return false;
}

#ifdef GCH_HAS_CONSTEXPR_SMALL_VECTOR

constexpr
int
test_func0 (void)
{
  small_vector<int> c { };
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func1 (void)
{
  small_vector<int> c (3, 2);
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func2 (void)
{
  small_vector<int> c (2);
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func3 (void)
{
  std::array<int, 3> a { 2, 4, 6 };
  small_vector<int> c (a.begin (), a.end ());
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func4 (void)
{
  small_vector<int> c { 1, 2, 3 };
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func5 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func6 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  c.emplace (c.begin () + 1, 7);
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func7 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  small_vector<test_types::non_trivial> d { 7, 8, 9 };
  c = d;
  return std::accumulate (c.begin (), c.end (), 0);
}

constexpr
int
test_func8 (void)
{
  small_vector<test_types::non_trivial> c { 1, 2, 3 };
  small_vector<test_types::non_trivial> d { 7, 8, 9 };
  c = std::move (d);
  return std::accumulate (c.begin (), c.end (), 0);
}

#endif

template <typename T, typename Allocator = std::allocator<T>,
          typename InlineCapacityType =
            std::integral_constant<unsigned, default_buffer_size<Allocator>::value>>
using small_vector_ht = small_vector<T, InlineCapacityType::value, Allocator>;

template <template <typename ...> class VectorT>
void
f (void)
{
  VectorT<int> x;
  VectorT<std::string> y;
  (void)x;
  (void)y;
  /* ... */
}

static
void
g (void)
{
  f<std::vector> ();
  f<small_vector_ht> ();
}

static
void
test_nonmember (void)
{
  small_vector<int> v { 1, 2, 3 };
  const auto& cv = v;

  assert (v.begin ()  == begin (v));
  assert (cv.begin () == begin (cv));
  assert (v.cbegin () == cbegin (v));

  assert (v.end ()  == end (v));
  assert (cv.end () == end (cv));
  assert (v.cend () == cend (v));

  assert (v.rbegin ()  == rbegin (v));
  assert (cv.rbegin () == rbegin (cv));
  assert (v.crbegin () == crbegin (v));

  assert (v.rend ()  == rend (v));
  assert (cv.rend () == rend (cv));
  assert (v.crend () == crend (v));

  assert (v.size () == size (v));
  assert (static_cast<std::ptrdiff_t> (v.size ()) == ssize (v));

  assert (v.empty () == empty (v));

  assert (v.data ()  == data (v));
  assert (cv.data () == data (cv));

  v.clear ();

  assert (v.empty () == empty (v));
}

static
void
test_disparate (void)
{
  small_vector<int, 3> p { 1, 2, 3 };
  small_vector<int, 2> p1 (p);

  small_vector<int, 5> q (std::move (p));
  small_vector<int, 4> q1 (std::move (q));
  const small_vector<int, 6> r { 1, 2, 3 };
  assert (q1 == r);
  assert (q1 == p1);
  assert (q1 != p);
}

static
void
test_alloc (void)
{
#ifdef GCH_LIB_CONCEPTS
  static_assert (concepts::Allocator<tiny_allocator<int>>);
#endif

  small_vector<int> vs;
  std::cout << "std::allocator<int>:"                         << '\n';
  std::cout << "  sizeof (vs):     " << sizeof (vs)           << '\n';
  std::cout << "  Inline capacity: " << vs.inline_capacity () << '\n';
  std::cout << "  Maximum size:    " << vs.max_size ()        << "\n\n";

  small_vector<int, default_buffer_size<tiny_allocator<int>>::value, tiny_allocator<int>> vt;
  std::cout << "tiny_allocator<int>:"                         << '\n';
  std::cout << "  sizeof (vt):     " << sizeof (vt)           << '\n';
  std::cout << "  Inline capacity: " << vt.inline_capacity () << '\n';
  std::cout << "  Maximum size:    " << vt.max_size ()        << std::endl;
}

class assignable_from
{
public:
  assignable_from            (void)                       = default;
  assignable_from            (const assignable_from&)     = default;
  assignable_from            (assignable_from&&) noexcept = default;
  assignable_from& operator= (const assignable_from&)     = default;
  assignable_from& operator= (assignable_from&&) noexcept = default;
  ~assignable_from           (void)                       = default;

  assignable_from (double d) noexcept
    : x (static_cast<int> (d))
  { }

  assignable_from&
  operator= (double d) noexcept
  {
    x = static_cast<int> (d);
    return *this;
  }

private:
  int x;
};

class not_assignable_from
{
public:
  not_assignable_from            (void)                           = default;
  not_assignable_from            (const not_assignable_from&)     = default;
  not_assignable_from            (not_assignable_from&&) noexcept = default;
  not_assignable_from& operator= (const not_assignable_from&)     = default;
  not_assignable_from& operator= (not_assignable_from&&) noexcept = default;
  ~not_assignable_from           (void)                           = default;

  not_assignable_from (double d) noexcept
    : x (static_cast<int> (d))
  {
    (void) x;
  }

  assignable_from&
  operator= (double d) = delete;

private:
  int x;
};

int
main (void)
{
  small_vector<double, default_buffer_size<tiny_allocator<double>>::value, tiny_allocator<double>> x;
  g ();

#ifdef GCH_HAS_CONSTEXPR_SMALL_VECTOR

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

  std::array<double, 3> ad { 1.0, 2.0, 3.0 };

  small_vector<assignable_from> af;
  af.assign (ad.begin (), ad.end ());

  small_vector<not_assignable_from> naf;
  naf.assign (ad.begin (), ad.end ());

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

  std::cout << "1:" << sizeof (small_vector<std::uint16_t, 6, tiny_allocator<std::uint16_t>>) << std::endl;
  std::cout << "2:" << sizeof (small_vector<std::uint16_t, 0, tiny_allocator<std::uint16_t>>) << std::endl;

  std::cout << static_cast<std::size_t> (x.max_size ()) << std::endl;
  std::cout << sizeof (small_vector<double, 0, tiny_allocator<double>>) << std::endl << std::endl;

  std::cout << default_buffer_size<tiny_allocator<double>>::value << std::endl;
  std::cout << alignof (detail::inline_storage<std::uint16_t, 6>) << std::endl;
  std::cout << alignof (detail::inline_storage<std::uint16_t, 0>) << std::endl;
  std::cout << sizeof (small_vector<std::uint16_t, 0, tiny_allocator<std::uint16_t>>) << std::endl;
  std::cout << sizeof (gch::detail::small_vector_base<tiny_allocator<std::uint16_t>, 6>) << std::endl;

  std::cout << sizeof (small_vector<double, 0, std::allocator<double>>) << std::endl;

  try
  {
    x.assign (128, 0.3);
  }
  catch (...)
  {
    std::cout << "successfully caught" << std::endl;
  }

  small_vector<int> w (std::move (v));
  small_vector<int> ww (v);

  test_nonmember ();
  test_disparate ();
  test_alloc ();
  return 0;
}
