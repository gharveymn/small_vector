/** small_vector.hpp
 * An implementation of `small_vector` (a vector with a small
 * buffer optimization). I would probably have preferred to
 * call this `inline_vector`, but I'll just go with the canonical
 * name for now.
 *
 * Copyright © 2020 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GCH_SMALL_VECTOR_HPP
#define GCH_SMALL_VECTOR_HPP

#ifdef __clang__
#    define GCH_CLANG
#  if defined (__cplusplus) && __cplusplus >= 202002L
#    define GCH_CLANG_20
#  endif
#  if defined (__cplusplus) && __cplusplus >= 201703L
#    define GCH_CLANG_17
#  endif
#  if defined (__cplusplus) && __cplusplus >= 201402L
#    define GCH_CLANG_14
#  endif
#  if defined (__cplusplus) && __cplusplus >= 201103L
#    define GCH_CLANG_11
#  endif
#endif

#ifdef __cpp_constexpr
#  ifndef GCH_CPP14_CONSTEXPR
#    if __cpp_constexpr >= 201304L
#      define GCH_CPP14_CONSTEXPR constexpr
#      ifndef GCH_HAS_CPP14_CONSTEXPR
#        define GCH_HAS_CPP14_CONSTEXPR
#      endif
#    else
#      define GCH_CPP14_CONSTEXPR
#    endif
#  endif
#  ifndef GCH_CPP17_CONSTEXPR
#    if __cpp_constexpr >= 201603L
#      define GCH_CPP17_CONSTEXPR constexpr
#      ifndef GCH_HAS_CPP17_CONSTEXPR
#        define GCH_HAS_CPP17_CONSTEXPR
#      endif
#    else
#      define GCH_CPP17_CONSTEXPR
#    endif
#  endif
#  ifndef GCH_CPP20_CONSTEXPR
#    if __cpp_constexpr >= 201907L
#      define GCH_CPP20_CONSTEXPR constexpr
#      ifndef GCH_HAS_CPP20_CONSTEXPR
#        define GCH_HAS_CPP20_CONSTEXPR
#      endif
#    else
#      define GCH_CPP20_CONSTEXPR
#    endif
#  endif
#endif

#ifndef GCH_INLINE_VAR
#  if defined (__cpp_inline_variables) && __cpp_inline_variables >= 201606
#    define GCH_INLINE_VAR inline
#  else
#    define GCH_INLINE_VAR
#  endif
#endif

#ifndef GCH_NORETURN
#  if defined (__has_cpp_attribute) && __has_cpp_attribute (noreturn) >= 200809L
#    define GCH_NORETURN [[noreturn]]
#  else
#    define GCH_NORETURN
#  endif
#endif

#ifndef GCH_NODISCARD
#  if defined (__has_cpp_attribute) && __has_cpp_attribute (nodiscard) >= 201603L
#    if ! defined (__clang__) || defined (GCH_CLANG_17)
#      define GCH_NODISCARD [[nodiscard]]
#    else
#      define GCH_NODISCARD
#    endif
#  else
#    define GCH_NODISCARD
#  endif
#endif

#ifndef GCH_EMPTY_BASE
#  if defined (_MSC_VER) && _MSC_VER >= 1910
#    define GCH_EMPTY_BASE __declspec(empty_bases)
#  else
#    define GCH_EMPTY_BASE
#  endif
#endif

#if defined (__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
#  ifndef GCH_IMPL_THREE_WAY_COMPARISON
#    define GCH_IMPL_THREE_WAY_COMPARISON
#  endif
#  if __has_include(<compare>)
#    include <compare>
#    if defined (__cpp_lib_three_way_comparison) && __cpp_lib_three_way_comparison >= 201907L
#      ifndef GCH_LIB_THREE_WAY_COMPARISON
#        define GCH_LIB_THREE_WAY_COMPARISON
#      endif
#    endif
#  endif
#endif

#if defined (__cpp_variable_templates) && __cpp_variable_templates >= 201304
#  ifndef GCH_VARIABLE_TEMPLATES
#    define GCH_VARIABLE_TEMPLATES
#  endif
#endif

#if defined (__cpp_lib_erase_if) && __cpp_lib_erase_if >= 202002L
#  ifndef GCH_LIB_ERASE_IF
#    define GCH_LIB_ERASE_IF
#  endif
#endif

#if defined (__cpp_deduction_guides) && __cpp_deduction_guides >= 201703
#  ifndef GCH_CTAD_SUPPORT
#    define GCH_CTAD_SUPPORT
#  endif
#endif

#if defined (__cpp_exceptions) && __cpp_exceptions >= 199711L
#  ifndef GCH_EXCEPTIONS
#    define GCH_EXCEPTIONS
#  endif
#endif

#if defined (__cpp_lib_is_final) && __cpp_lib_is_final >= 201402L
#  ifndef GCH_LIB_IS_FINAL
#    define GCH_LIB_IS_FINAL
#  endif
#endif

#if defined (__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated >= 201811L
#  ifndef GCH_LIB_IS_CONSTANT_EVALUATED
#    define GCH_LIB_IS_CONSTANT_EVALUATED
#  endif
#endif

#if defined (__cpp_lib_allocator_traits_is_always_equal)
#  if __cpp_lib_allocator_traits_is_always_equal >= 201411L
#    ifndef GCH_LIB_IS_ALWAYS_EQUAL
#      define GCH_LIB_IS_ALWAYS_EQUAL
#    endif
#  endif
#endif

#if defined (__cpp_lib_is_swappable) && __cpp_lib_is_swappable >= 201603L
#  ifndef GCH_LIB_IS_SWAPPABLE
#    define GCH_LIB_IS_SWAPPABLE
#  endif
#endif

#if defined (__cpp_concepts) && __cpp_concepts >= 201907L
#  ifndef GCH_CONCEPTS
#    define GCH_CONCEPTS
#  endif
#  if defined(__has_include) && __has_include(<concepts>)
#    include <concepts>
#    if defined (__cpp_lib_concepts) && __cpp_lib_concepts >= 202002L
#      if ! defined (GCH_LIB_CONCEPTS) && ! defined (GCH_DISABLE_CONCEPTS)
#        define GCH_LIB_CONCEPTS
#      endif
#    endif
#  endif
#endif

#if defined (__cpp_constinit) && __cpp_constinit >= 201907L
#  ifndef GCH_CONSTINIT
#    define GCH_CONSTINIT
#  endif
#endif

#include <algorithm>
#include <cassert>
#include <cstring>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>

#if defined (__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 201811L
#  ifndef GCH_LIB_CONSTEXPR_MEMORY
#    define GCH_LIB_CONSTEXPR_MEMORY
#  endif
#endif

#ifdef GCH_STDLIB_INTEROP
#  include <array>
#  include <valarray>
#  include <vector>
#endif

#ifdef GCH_EXCEPTIONS
#  include <stdexcept>
#endif

#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
#  define GCH_ASSERT(EVAL) \
(! std::is_constant_evaluated () ? assert(EVAL) : static_cast<void> (0))
#else
#  define GCH_ASSERT(EVAL) \
assert (EVAL)
#endif

// defined if the entire thing is available for constexpr
#ifdef GCH_HAS_CPP20_CONSTEXPR
#  if defined (GCH_LIB_IS_CONSTANT_EVALUATED) && defined (GCH_LIB_CONSTEXPR_MEMORY)
#    define GCH_CONSTEXPR_SMALL_VECTOR
#  endif
#endif

namespace gch
{

#ifdef GCH_LIB_CONCEPTS

  namespace concepts
  {

    // Note: this mirrors the named requirements, not the standard concepts, so we don't require
    // the destructor to be noexcept for Destructible.

    template <typename T>
    concept Destructible = std::is_destructible<T>::value;

    template <typename T>
    concept TriviallyDestructible = std::is_trivially_destructible<T>::value;

    template <typename T>
    concept NoThrowDestructible = std::is_nothrow_destructible<T>::value;

    // Note: this mirrors the named requirements, not the standard library concepts,
    // so we don't require Destructible here.

    template <typename T, typename... Args>
    concept ConstructibleFrom = std::is_constructible<T, Args...>::value;

    template <typename T, typename... Args>
    concept NoThrowConstructibleFrom = std::is_nothrow_constructible<T, Args...>::value;

    template <typename From, typename To>
    concept ConvertibleTo =
          std::is_convertible<From, To>::value
      &&  requires (typename std::add_rvalue_reference<From>::type (&f) ())
          {
            static_cast<To> (f ());
          };

    template <typename From, typename To>
    concept NoThrowConvertibleTo =
          std::is_nothrow_convertible<From, To>::value
      &&  requires (typename std::add_rvalue_reference<From>::type (&f) () noexcept)
          {
            { static_cast<To> (f ()) } noexcept;
          };

    // Note: std::default_initializable requires std::destructible.
    template <typename T>
    concept DefaultConstructible =
          ConstructibleFrom<T>
      &&  requires { T { }; }
      &&  requires { ::new (static_cast<void *> (nullptr)) T; };

    template <typename T>
    concept MoveAssignable = std::assignable_from<T&, T>;

    template <typename T>
    concept CopyAssignable =
          MoveAssignable<T>
      &&  std::assignable_from<T&, T&>
      &&  std::assignable_from<T&, const T&>
      &&  std::assignable_from<T&, const T>;

    template <typename T>
    concept MoveConstructible = ConstructibleFrom<T, T> && ConvertibleTo<T, T>;

    template <typename T>
    concept NoThrowMoveConstructible =
          NoThrowConstructibleFrom<T, T>
      &&  NoThrowConvertibleTo<T, T>;

    template <typename T>
    concept CopyConstructible =
          MoveConstructible<T>
      &&  ConstructibleFrom<T,       T&> && ConvertibleTo<      T&, T>
      &&  ConstructibleFrom<T, const T&> && ConvertibleTo<const T&, T>
      &&  ConstructibleFrom<T, const T > && ConvertibleTo<const T , T>;

    template <typename T>
    concept NoThrowCopyConstructible =
          NoThrowMoveConstructible<T>
      &&  NoThrowConstructibleFrom<T,       T&> && NoThrowConvertibleTo<      T&, T>
      &&  NoThrowConstructibleFrom<T, const T&> && NoThrowConvertibleTo<const T&, T>
      &&  NoThrowConstructibleFrom<T, const T > && NoThrowConvertibleTo<const T , T>;

    template <typename T>
    concept Swappable = std::swappable<T>;

    template <typename T>
    concept EqualityComparable = std::equality_comparable<T>;


    // if X::allocator_type then
    //   std::same_as<typename X::allocator_type,
    //                typename std::allocator_traits<A>::template rebind_alloc<T>>
    // otherwise
    //   no condition; we use std::allocator<T> regardless of A
    //
    // see [22.2.1].16
    template <typename T, typename X, typename A, typename ...Args>
    concept EmplaceConstructible =
          std::same_as<typename X::value_type, T>
      &&  (  (  requires { typename X::allocator_type; } // only perform this check if X is
            &&  std::same_as<typename X::allocator_type, // allocator-aware
                             typename std::allocator_traits<A>::template rebind_alloc<T>>
            &&  requires (A m, T *p, Args&&... args)
                {
                  std::allocator_traits<A>::construct (m, p, std::forward<Args> (args)...);
                })
         ||  (! requires { typename X::allocator_type; }
            &&  requires (typename std::allocator<T> m, T *p, Args&&... args)
                { // If X is not allocator aware we just use allocator<T>
                  std::allocator_traits<std::allocator<T>>::construct (
                    m, p, std::forward<Args> (args)...);
                }));


    // T is a type
    // X is a Container
    // A is an Allocator
    // if A is std::allocator<T> then
    template <typename T, typename X,
              typename A = typename std::conditional<requires { typename X::allocator_type; },
                                                    typename X::allocator_type,
                                                    std::allocator<T>>::type>
    concept DefaultInsertable = EmplaceConstructible<T, X, A>;

    template <typename T, typename X,
              typename A = typename std::conditional<requires { typename X::allocator_type; },
                                                    typename X::allocator_type,
                                                    std::allocator<T>>::type>
    concept MoveInsertable = EmplaceConstructible<T, X, A, T>;

    template <typename T, typename X,
              typename A = typename std::conditional<requires { typename X::allocator_type; },
                                                    typename X::allocator_type,
                                                    std::allocator<T>>::type>
    concept CopyInsertable = EmplaceConstructible<T, X, A,       T&>
                         &&  EmplaceConstructible<T, X, A, const T&>;

    // same method as with EmplaceConstructible
    template <typename T, typename X,
              typename A = typename std::conditional<requires { typename X::allocator_type; },
                                                    typename X::allocator_type,
                                                    std::allocator<T>>::type>
    concept Erasable =
          std::same_as<typename X::value_type, T>
      &&  (  (  requires { typename X::allocator_type; } // if X is allocator aware
            &&  std::same_as<typename X::allocator_type,
                             typename std::allocator_traits<A>::template rebind_alloc<T>>
            &&  requires (A m, T *p)
                {
                  std::allocator_traits<A>::destroy (m, p);
                })
         ||  (! requires { typename X::allocator_type; }
            &&  requires (typename std::allocator<T> m, T *p)
                {
                  std::allocator_traits<std::allocator<T>>::destroy (m, p);
                }));

    template <typename T>
    concept ContextuallyConvertibleToBool = std::constructible_from<bool, T>;

    template <typename T>
    concept BoolConstant = std::derived_from<T, std::true_type>
                        || std::derived_from<T, std::false_type>;

    template <typename T>
    concept NullablePointer =
          EqualityComparable<T>
      &&  DefaultConstructible<T>
      &&  CopyConstructible<T>
      &&  CopyAssignable<T>
      &&  Destructible<T>
      &&  ConstructibleFrom<T, std::nullptr_t>
      &&  ConvertibleTo<std::nullptr_t, T>
      &&  requires (T p, T q, std::nullptr_t np)
          {
            { T (np)   } -> std::same_as<T>;
            { p = np   } -> std::same_as<T&>;
            { p  != q  } -> ContextuallyConvertibleToBool;
            { p  == np } -> ContextuallyConvertibleToBool;
            { np == p  } -> ContextuallyConvertibleToBool;
            { p  != np } -> ContextuallyConvertibleToBool;
            { np != p  } -> ContextuallyConvertibleToBool;
          };

    static_assert(  NullablePointer<int *>);
    static_assert(! NullablePointer<int>);

    template <typename A,
              typename U = typename std::allocator_traits<A>::value_type,
              typename B = typename std::allocator_traits<A>::template rebind_alloc<U>>
    concept Allocator =
          NoThrowCopyConstructible<A>
      &&  requires (A a,
                    B b,
                    U *xp,
                    typename std::allocator_traits<A>::pointer p,
                    typename std::allocator_traits<A>::const_pointer cp,
                    typename std::allocator_traits<A>::void_pointer vp,
                    typename std::allocator_traits<A>::const_void_pointer cvp,
                    typename std::allocator_traits<A>::value_type& r,
                    typename std::allocator_traits<A>::size_type n)
          {
            /** Inner types **/
            // A::pointer
            requires NullablePointer<            typename std::allocator_traits<A>::pointer>;
            requires std::random_access_iterator<typename std::allocator_traits<A>::pointer>;
            requires std::contiguous_iterator<   typename std::allocator_traits<A>::pointer>;

            // A::const_pointer
            requires NullablePointer<            typename std::allocator_traits<A>::const_pointer>;
            requires std::random_access_iterator<typename std::allocator_traits<A>::const_pointer>;
            requires std::contiguous_iterator<   typename std::allocator_traits<A>::const_pointer>;

            requires std::convertible_to<typename std::allocator_traits<A>::pointer,
                                         typename std::allocator_traits<A>::const_pointer>;

            // A::void_pointer
            requires NullablePointer<typename std::allocator_traits<A>::void_pointer>;

            requires std::convertible_to<typename std::allocator_traits<A>::pointer,
                                         typename std::allocator_traits<A>::void_pointer>;

            requires std::same_as<typename std::allocator_traits<A>::void_pointer,
                                  typename std::allocator_traits<B>::void_pointer>;

            // A::const_void_pointer
            requires NullablePointer<typename std::allocator_traits<A>::const_void_pointer>;

            requires std::convertible_to<typename std::allocator_traits<A>::pointer,
                                         typename std::allocator_traits<A>::const_void_pointer>;

            requires std::convertible_to<typename std::allocator_traits<A>::const_pointer,
                                         typename std::allocator_traits<A>::const_void_pointer>;

            requires std::convertible_to<typename std::allocator_traits<A>::void_pointer,
                                         typename std::allocator_traits<A>::const_void_pointer>;

            requires std::same_as<typename std::allocator_traits<A>::const_void_pointer,
                                  typename std::allocator_traits<B>::const_void_pointer>;

            // A::value_type
            typename A::value_type;
            requires std::same_as<typename A::value_type,
                                  typename std::allocator_traits<A>::value_type>;

            // A::size_type
            requires std::unsigned_integral<typename std::allocator_traits<A>::size_type>;

            // A::difference_type
            requires std::signed_integral<typename std::allocator_traits<A>::difference_type>;

            // A::template rebind<U>::other
            requires std::same_as<A,
              typename std::allocator_traits<B>::template rebind_alloc<typename A::value_type>>;

            /** Operations on pointers **/
            { *p  } -> std::same_as<typename A::value_type&>;
            { *cp } -> std::same_as<const typename A::value_type&>;

            // Language in the standard implies that `decltype (p)` must either
            // be a raw pointer or implement `operator->`. There is no mention
            // of `std::to_address` or `std::pointer_traits<Ptr>::to_address`
            requires std::same_as<decltype (p), typename A::value_type *>
                 ||  requires
                     {
                       { p.operator-> () } -> std::same_as<typename A::value_type *>;
                     };

            requires std::same_as<decltype (cp), const typename A::value_type *>
                 ||  requires
                     {
                       { cp.operator-> () } -> std::same_as<const typename A::value_type *>;
                     };

            { static_cast<decltype (p)> (vp)   } -> std::same_as<decltype (p)>;
            { static_cast<decltype (cp)> (cvp) } -> std::same_as<decltype (cp)>;

            { std::pointer_traits<decltype (p)>::pointer_to (r) } -> std::same_as<decltype (p)>;

            /** Storage and lifetime operations **/
            // a.allocate (n)
            { a.allocate (n) } -> std::same_as<decltype (p)>;

            // a.allocate(n, cvp) [optional]
            requires ! requires { a.allocate (n, cvp); }
                   ||  requires { { a.allocate (n, cvp) } -> std::same_as<decltype (p)>; };

            // a.deallocate(p, n)
            { a.deallocate (p, n) } -> std::convertible_to<void>;

            // a.max_size () [optional]
            requires ! requires { a.max_size (); }
                   ||  requires { { a.max_size () } -> std::same_as<decltype (n)>; };

            // a.construct (xp, args) [optional]
            requires ! requires { a.construct (xp); }
                   ||  requires { { a.construct (xp) } -> std::convertible_to<void>; };

            // a.destroy (xp) [optional]
            requires ! requires { a.destroy (xp); }
                   ||  requires { { a.destroy (xp) } -> std::convertible_to<void>; };

            /** Relationship between instances **/
            requires NoThrowConstructibleFrom<A, decltype (b)>;
            requires NoThrowConstructibleFrom<A, decltype (std::move (b))>;

            requires BoolConstant<typename std::allocator_traits<A>::is_always_equal>;

            /** Influence on container operations **/
            // a.select_on_container_copy_construction () [optional]
            requires ! requires { a.select_on_container_copy_construction (); }
                   ||  requires
                       {
                         { a.select_on_container_copy_construction () } -> std::same_as<A>;
                       };

            requires BoolConstant<
              typename std::allocator_traits<A>::propagate_on_container_copy_assignment>;

            requires BoolConstant<
              typename std::allocator_traits<A>::propagate_on_container_move_assignment>;

            requires BoolConstant<
              typename std::allocator_traits<A>::propagate_on_container_swap>;

          }
      &&  requires (A a1, A a2)
          {
            { a1 == a2 } noexcept -> std::same_as<bool>;
            { a1 != a2 } noexcept -> std::same_as<bool>;
          };

    static_assert (Allocator<std::allocator<int>>,
                  "std::allocator<int> failed to meet Allocator concept requirements.");

  } // namespace concepts

#endif

  template <typename Allocator>
  struct default_buffer_size;

  template <typename T,
            unsigned InlineCapacity = default_buffer_size<std::allocator<T>>::value,
            typename Allocator      = std::allocator<T>>
#ifdef GCH_LIB_CONCEPTS
  requires concepts::Allocator<Allocator, T>
#endif
  class small_vector;

  template <typename Allocator>
  struct default_buffer_size
  {
    using value_type = typename std::allocator_traits<Allocator>::value_type;

    static constexpr
    unsigned
    ideal_buffer_max = 256;

    static constexpr
    unsigned
    ideal_total      = 64;

#ifndef GCH_UNRESTRICTED_DEFAULT_BUFFER_SIZE
    // FIXME: Some compilers will not emit the error from this static_assert
    //        while instantiating a small_vector, and attribute the mistake
    //        to some random other function.
    // static_assert (sizeof (value_type) <= ideal_buffer_max, "`sizeof(T)` too large");
#endif

    static constexpr
    unsigned
    ideal_buffer = ideal_total - sizeof (small_vector<value_type, 0, Allocator>);

    static_assert (ideal_buffer < ideal_total,
                   "small_vector<T, 0, Allocator> is larger than ideal_total!");

    static constexpr
    unsigned
    value = (sizeof (value_type) <= ideal_buffer) ? (ideal_buffer / sizeof (value_type)) : 1;
  };

#ifdef GCH_VARIABLE_TEMPLATES
  template <typename Allocator>
  GCH_INLINE_VAR constexpr unsigned default_buffer_size_v = default_buffer_size<Allocator>::value;
#endif

  template <typename Pointer, typename Difference>
  class small_vector_iterator
  {
    using traits = std::iterator_traits<Pointer>;

  public:
    using difference_type   = Difference;
    using value_type        = typename traits::value_type;
    using pointer           = typename traits::pointer;
    using reference         = typename traits::reference;
    using iterator_category = typename traits::iterator_category;
#ifdef GCH_LIB_CONCEPTS
    using iterator_concept  = std::contiguous_iterator_tag;
#endif

//  small_vector_iterator            (void)                             = impl;
    small_vector_iterator            (const small_vector_iterator&)     = default;
    small_vector_iterator            (small_vector_iterator&&) noexcept = default;
    small_vector_iterator& operator= (const small_vector_iterator&)     = default;
    small_vector_iterator& operator= (small_vector_iterator&&) noexcept = default;
    ~small_vector_iterator           (void)                             = default;

#ifdef NDEBUG
    small_vector_iterator (void) = default;
#else
    constexpr
    small_vector_iterator (void) noexcept
      : m_ptr ()
    { }
#endif

    constexpr explicit
    small_vector_iterator (const Pointer& p) noexcept
      : m_ptr (p)
    { }

    template <typename U, typename D,
      typename std::enable_if<std::is_convertible<U, Pointer>::value>::type * = nullptr>
    constexpr /* implicit */
    small_vector_iterator (const small_vector_iterator<U, D>& other) noexcept
      : m_ptr (other.base ())
    { }

    GCH_CPP14_CONSTEXPR
    small_vector_iterator&
    operator++ (void) noexcept
    {
      ++m_ptr;
      return *this;
    }

    GCH_CPP14_CONSTEXPR
    small_vector_iterator
    operator++ (int) noexcept
    {
      return small_vector_iterator (m_ptr++);
    }

    GCH_CPP14_CONSTEXPR
    small_vector_iterator&
    operator-- (void) noexcept
    {
      --m_ptr;
      return *this;
    }

    GCH_CPP14_CONSTEXPR
    small_vector_iterator
    operator-- (int) noexcept
    {
      return small_vector_iterator (m_ptr--);
    }

    GCH_CPP14_CONSTEXPR
    small_vector_iterator&
    operator+= (difference_type n) noexcept
    {
      m_ptr += n;
      return *this;
    }

    constexpr
    small_vector_iterator
    operator+ (difference_type n) const noexcept
    {
      return small_vector_iterator (m_ptr + n);
    }

    GCH_CPP14_CONSTEXPR
    small_vector_iterator&
    operator-= (difference_type n) noexcept
    {
      m_ptr -= n;
      return *this;
    }

    constexpr
    small_vector_iterator
    operator- (difference_type n) const noexcept
    {
      return small_vector_iterator (m_ptr - n);
    }

    constexpr
    reference operator* (void) const noexcept
    {
      return *m_ptr;
    }

    constexpr
    pointer
    operator-> (void) const noexcept
    {
      return m_ptr;
    }

    constexpr
    reference
    operator[] (difference_type n) const noexcept
    {
      return m_ptr[n];
    }

    constexpr
    const Pointer&
    base () const noexcept
    {
      return m_ptr;
    }

    /* comparisons */

  private:
    Pointer m_ptr;
  };

#ifdef GCH_LIB_THREE_WAY_COMPARISON

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  bool
  operator== (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
              const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs)
    noexcept (noexcept (lhs.base () == rhs.base ()))
    requires requires { { lhs.base () == rhs.base () } -> std::convertible_to<bool>; }
  {
    return lhs.base () == rhs.base ();
  }

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  auto
  operator<=> (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
               const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs)
    noexcept (noexcept (lhs.base () <=> rhs.base ()))
    requires std::three_way_comparable_with<PointerLHS, PointerRHS>
  {
    return lhs.base () <=> rhs.base ();
  }

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  auto
  operator<=> (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
               const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs)
    noexcept (noexcept (lhs.base () < rhs.base ()) && noexcept (rhs.base () < lhs.base ()))
    requires (! std::three_way_comparable_with<PointerLHS, PointerRHS>)
  {
    using ordering = std::weak_ordering;
    return (lhs.base () < rhs.base ()) ? ordering::less
                                       : (rhs.base () < lhs.base ()) ? ordering::greater
                                                                     : ordering::equivalent;
  }

#else

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  bool
  operator== (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
              const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs) noexcept
  {
    return lhs.base () == rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  bool
  operator== (const small_vector_iterator<Pointer, Difference>& lhs,
              const small_vector_iterator<Pointer, Difference>& rhs) noexcept
  {
    return lhs.base () == rhs.base ();
  }

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  bool
  operator!= (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
              const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs) noexcept
  {
    return lhs.base () != rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  bool
  operator!= (const small_vector_iterator<Pointer, Difference>& lhs,
              const small_vector_iterator<Pointer, Difference>& rhs) noexcept
  {
    return lhs.base () != rhs.base ();
  }

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  bool
  operator< (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
             const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs) noexcept
  {
    return lhs.base () < rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  bool
  operator< (const small_vector_iterator<Pointer, Difference>& lhs,
             const small_vector_iterator<Pointer, Difference>& rhs) noexcept
  {
    return lhs.base () < rhs.base ();
  }

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  bool
  operator> (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
             const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs) noexcept
  {
    return lhs.base () > rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  bool
  operator> (const small_vector_iterator<Pointer, Difference>& lhs,
             const small_vector_iterator<Pointer, Difference>& rhs) noexcept
  {
    return lhs.base () > rhs.base ();
  }

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  bool
  operator<= (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
              const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs) noexcept
  {
    return lhs.base () <= rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  bool
  operator<= (const small_vector_iterator<Pointer, Difference>& lhs,
              const small_vector_iterator<Pointer, Difference>& rhs) noexcept
  {
    return lhs.base () <= rhs.base ();
  }

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  bool
  operator>= (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
              const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs) noexcept
  {
    return lhs.base () >= rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  bool
  operator>= (const small_vector_iterator<Pointer, Difference>& lhs,
              const small_vector_iterator<Pointer, Difference>& rhs) noexcept
  {
    return lhs.base () >= rhs.base ();
  }

#endif

  template <typename PointerLHS, typename DifferenceLHS,
            typename PointerRHS, typename DifferenceRHS>
  constexpr
  auto
  operator- (const small_vector_iterator<PointerLHS, DifferenceLHS>& lhs,
                  const small_vector_iterator<PointerRHS, DifferenceRHS>& rhs) noexcept
    -> decltype (lhs.base () - rhs.base ())
  {
    return lhs.base () - rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  auto
  operator- (const small_vector_iterator<Pointer, Difference>& lhs,
             const small_vector_iterator<Pointer, Difference>& rhs) noexcept
    -> decltype (lhs.base () - rhs.base ())
  {
    return lhs.base () - rhs.base ();
  }

  template <typename Pointer, typename Difference>
  constexpr
  small_vector_iterator<Pointer, Difference>
  operator+ (typename small_vector_iterator<Pointer, Difference>::difference_type n,
             const small_vector_iterator<Pointer, Difference>& it) noexcept
  {
    return it + n;
  }

  namespace detail
  {

#ifndef GCH_LIB_IS_SWAPPABLE
    namespace small_vector_adl
    {

      using std::swap;

      template <typename T, typename Enable = void>
      struct is_nothrow_swappable
        : std::false_type
      { };

      template <typename T>
      struct is_nothrow_swappable<T, decltype (swap (std::declval<T&> (), std::declval<T&> ()))>
        : std::integral_constant<bool, noexcept (swap (std::declval<T&> (), std::declval<T&> ()))>
      { };

    }
#endif

    template <typename T, unsigned InlineCapacity>
    class inline_storage
    {
    public:
      using value_t = T;

      inline_storage            (void)                      = default;
      inline_storage            (const inline_storage&)     = delete;
      inline_storage            (inline_storage&&) noexcept = delete;
      inline_storage& operator= (const inline_storage&)     = delete;
      inline_storage& operator= (inline_storage&&) noexcept = delete;
      ~inline_storage           (void)                      = default;

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      value_t *
      get_inline_ptr (void) noexcept
      {
        return static_cast<value_t *> (static_cast<void *> (std::addressof (*m_data)));
      }

      GCH_NODISCARD constexpr
      const value_t *
      get_inline_ptr (void) const noexcept
      {
        return static_cast<const value_t *> (static_cast<const void *> (std::addressof (*m_data)));
      }

      static constexpr
      std::size_t
      element_size (void) noexcept
      {
        return sizeof (value_t);
      }

      static constexpr
      std::size_t
      alignment (void) noexcept
      {
        return alignof (value_t);
      }

      static constexpr
      unsigned
      num_elements (void) noexcept
      {
        return InlineCapacity;
      }

      static constexpr
      std::size_t
      num_bytes (void) noexcept
      {
        return num_elements () * element_size ();
      }

    private:
      typename std::aligned_storage<element_size (), alignment ()>::type m_data[num_elements ()];
    };

    template <typename T>
    class GCH_EMPTY_BASE inline_storage<T, 0>
    {
    public:
      using value_t = T;

      inline_storage            (void)                      = default;
      inline_storage            (const inline_storage&)     = delete;
      inline_storage            (inline_storage&&) noexcept = delete;
      inline_storage& operator= (const inline_storage&)     = delete;
      inline_storage& operator= (inline_storage&&) noexcept = delete;
      ~inline_storage           (void)                      = default;

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      value_t *
      get_inline_ptr (void) noexcept
      {
        return nullptr;
      }

      GCH_NODISCARD constexpr
      const value_t *
      get_inline_ptr (void) const noexcept
      {
        return nullptr;
      }

      static constexpr
      std::size_t
      element_size (void) noexcept
      {
        return sizeof (value_t);
      }

      static constexpr
      std::size_t
      alignment (void) noexcept
      {
        return alignof (value_t);
      }

      static constexpr
      unsigned
      num_elements (void) noexcept
      {
        return 0;
      }

      static constexpr
      std::size_t
      num_bytes (void) noexcept
      {
        return 0;
      }
    };

    template <typename Allocator, bool AvailableForEBO = std::is_empty<Allocator>::value
#ifdef GCH_LIB_IS_FINAL
                                                     &&! std::is_final<Allocator>::value
#endif // if using with C++11 just don't use an allocator marked as final :P
                                                         >
    class allocator_inliner;

    template <typename Allocator>
    class GCH_EMPTY_BASE allocator_inliner<Allocator, true>
      : private Allocator
    {
      using alloc_traits = std::allocator_traits<Allocator>;

      static constexpr
      bool
      copy_assign_is_noop = alloc_traits::propagate_on_container_copy_assignment::value;

      static constexpr
      bool
      move_assign_is_noop = alloc_traits::propagate_on_container_move_assignment::value;

      static constexpr
      bool
      swap_is_noop = alloc_traits::propagate_on_container_swap::value;

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (const allocator_inliner&) noexcept { }

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (const allocator_inliner& other)
      noexcept (noexcept (Allocator::operator= (other)))
      {
        Allocator::operator= (other);
      }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (allocator_inliner&&) noexcept { }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (allocator_inliner&& other)
      noexcept (noexcept (Allocator::operator= (std::move (other))))
      {
        Allocator::operator= (std::move (other));
      }

    public:
      allocator_inliner            (void)                         = default;
      allocator_inliner            (const allocator_inliner&)     = default;
      allocator_inliner            (allocator_inliner&&) noexcept = default;
//    allocator_inliner& operator= (const allocator_inliner&)     = impl;
//    allocator_inliner& operator= (allocator_inliner&&) noexcept = impl;
      ~allocator_inliner           (void)                         = default;

      constexpr explicit
      allocator_inliner (const Allocator& alloc)
        : Allocator (alloc)
      { }

      GCH_CPP20_CONSTEXPR
      allocator_inliner&
      operator= (const allocator_inliner& other)
        noexcept (noexcept (maybe_assign (other)))
      {
        GCH_ASSERT (&other != this
                &&  "`allocator_inliner` should not participate in self-copy-assignment.");
        maybe_assign (other);
        return *this;
      }

      GCH_CPP20_CONSTEXPR
      allocator_inliner&
      operator= (allocator_inliner&& other)
        noexcept (noexcept (maybe_assign (std::move (other))))
      {
        GCH_ASSERT (&other != this
                &&  "`allocator_inliner` should not participate in self-move-assignment.");
        maybe_assign (std::move (other));
        return *this;
      }

      static constexpr
      Allocator&
      get (allocator_inliner& self) noexcept
      {
        return self;
      }

      static constexpr
      const Allocator&
      get (const allocator_inliner& self) noexcept
      {
        return self;
      }

      template <bool IsNoOp = swap_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void
      swap (allocator_inliner&) { }

      template <bool IsNoOp = swap_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void
      swap (allocator_inliner& other)
      {
        using std::swap;
        swap (static_cast<Allocator&> (*this), static_cast<Allocator&> (other));
      }
    };

    template <typename Allocator>
    class allocator_inliner<Allocator, false>
    {
      using alloc_traits = std::allocator_traits<Allocator>;

      static constexpr
      bool
      copy_assign_is_noop = alloc_traits::propagate_on_container_copy_assignment::value;

      static constexpr
      bool
      move_assign_is_noop = alloc_traits::propagate_on_container_move_assignment::value;

      static constexpr
      bool
      swap_is_noop = alloc_traits::propagate_on_container_swap::value;

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (const allocator_inliner&) noexcept { }

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (const allocator_inliner& other)
        noexcept (noexcept (std::declval<decltype (other.m_alloc)&> () = other.m_alloc))
      {
        m_alloc = other.m_alloc;
      }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (allocator_inliner&&) noexcept { }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void
      maybe_assign (allocator_inliner&& other)
        noexcept (noexcept (std::declval<decltype (other.m_alloc)&> () = std::move (other.m_alloc)))
      {
        m_alloc = std::move (other.m_alloc);
      }

    public:
      allocator_inliner            (void)                         = default;
      allocator_inliner            (const allocator_inliner&)     = default;
      allocator_inliner            (allocator_inliner&&) noexcept = default;
//    allocator_inliner& operator= (const allocator_inliner&)     = impl;
//    allocator_inliner& operator= (allocator_inliner&&) noexcept = impl;
      ~allocator_inliner           (void)                         = default;

      GCH_CPP20_CONSTEXPR explicit
      allocator_inliner (const Allocator& alloc)
        : m_alloc (alloc)
      { }

      GCH_CPP20_CONSTEXPR
      allocator_inliner&
      operator= (const allocator_inliner& other)
        noexcept (noexcept (maybe_assign (other)))
      {
        GCH_ASSERT (&other != this
                &&  "`allocator_inliner` should not participate in self-copy-assignment.");
        maybe_assign (other);
        return *this;
      }

      GCH_CPP20_CONSTEXPR
      allocator_inliner&
      operator= (allocator_inliner&& other)
        noexcept (noexcept (maybe_assign (std::move (other))))
      {
        GCH_ASSERT (&other != this
                &&  "`allocator_inliner` should not participate in self-move-assignment.");
        maybe_assign (std::move (other));
        return *this;
      }

      static constexpr
      Allocator&
      get (allocator_inliner& self) noexcept
      {
        return self.m_alloc;
      }

      static constexpr
      const Allocator&
      get (const allocator_inliner& self) noexcept
      {
        return self.m_alloc;
      }

      template <bool IsNoOp = swap_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void
      swap (allocator_inliner&) { }

      template <bool IsNoOp = swap_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void
      swap (allocator_inliner& other)
      {
        using std::swap;
        swap (m_alloc, other.m_alloc);
      }

    private:
      Allocator m_alloc;
    };

    template <typename Allocator>
    class GCH_EMPTY_BASE allocator_interface
      : public allocator_inliner<Allocator>
    {
    public:
      using size_type = typename std::allocator_traits<Allocator>::size_type;

      // If difference_type is larger than size_type then we need
      // to rectify that problem.
      using difference_type = typename std::conditional<
        (
          static_cast<std::size_t> ((std::numeric_limits<size_type>::max) ())
          < // less-than
          static_cast<std::size_t> ((std::numeric_limits<
            typename std::allocator_traits<Allocator>::difference_type>::max) ())
        ),
        typename std::make_signed<size_type>::type,
        typename std::allocator_traits<Allocator>::difference_type>::type;

    private:
      using alloc_base = allocator_inliner<Allocator>;

    protected:
      using alloc_t      = Allocator;
      using alloc_traits = std::allocator_traits<alloc_t>;
      using value_t      = typename alloc_traits::value_type;
      using ptr          = typename alloc_traits::pointer;
      using cptr         = typename alloc_traits::const_pointer;
      using vptr         = typename alloc_traits::void_pointer;
      using cvptr        = typename alloc_traits::const_void_pointer;

      // select the fastest types larger than the user-facing types

      // FIXME: these are probably not portable
      using size_ty =
        typename std::conditional<
          (sizeof (size_type) == 1),
          std::uint_fast8_t,
          typename std::conditional<
            (sizeof (size_type) == 2),
            std::uint_fast16_t,
            typename std::conditional<
              (sizeof (size_type) <= 4),
              std::uint_fast32_t,
              std::uint_fast64_t
            >::type
          >::type
        >::type;

      using diff_ty =
        typename std::conditional<
          (sizeof (difference_type) == 1),
          std::int_fast8_t,
          typename std::conditional<
            (sizeof (difference_type) == 2),
            std::int_fast16_t,
            typename std::conditional<
              (sizeof (difference_type) <= 4),
              std::int_fast32_t,
              std::int_fast64_t
            >::type
          >::type
        >::type;

    private:
      template <typename ...>
      using void_t = void;

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

      template <typename, typename = void>
      struct has_ptr_traits_to_address
        : std::false_type
      { };

      template <typename P>
      struct has_ptr_traits_to_address<P,
            void_t<decltype (std::pointer_traits<P>::to_address (std::declval<P> ()))>>
        : std::true_type
      { };

      template <typename Void, typename A, typename V, typename ...Args>
      struct has_alloc_construct_impl
        : std::false_type
      { };

      template <typename A, typename V, typename ...Args>
      struct has_alloc_construct_impl<
            void_t<decltype (std::declval<A&> ().construct (std::declval<V *> (),
                                                            std::declval<Args> ()...))>,
            A, V, Args...>
        : std::true_type
      { };

      template <typename ...Args>
      struct has_alloc_construct
        : has_alloc_construct_impl<void, alloc_t, value_t, Args...>
      { };

      template <typename ...Args>
      struct must_use_alloc_construct
        : bool_constant<! std::is_same<alloc_t, std::allocator<value_t>>::value
                      &&  has_alloc_construct<alloc_t, Args...>::value>
      { };

      template <typename Void, typename A, typename V>
      struct has_alloc_destroy_impl
        : std::false_type
      { };

      template <typename A, typename V>
      struct has_alloc_destroy_impl<
            void_t<decltype (std::declval<A&> ().destroy (std::declval<V *> ()))>,
            A, V>
        : std::true_type
      { };

      template <typename A, typename V = value_t>
      struct has_alloc_destroy
        : has_alloc_destroy_impl<void, A, V>
      { };

      static constexpr
      bool
      has_alloc_destroy_v = has_alloc_destroy<alloc_t>::value;

      struct must_use_alloc_destroy
        : bool_constant<! std::is_same<alloc_t, std::allocator<value_t>>::value
                       &&  has_alloc_destroy_v>
      { };

      static constexpr
      bool
      must_use_alloc_destroy_v = must_use_alloc_destroy::value;

    public:
      allocator_interface            (void)                           = default;
//    allocator_interface            (const allocator_interface&)     = impl;
      allocator_interface            (allocator_interface&&) noexcept = default;
      allocator_interface& operator= (const allocator_interface&)     = default;
      allocator_interface& operator= (allocator_interface&&) noexcept = default;
      ~allocator_interface           (void)                           = default;

      GCH_CPP20_CONSTEXPR
      allocator_interface (const allocator_interface& other)
        : alloc_base (alloc_traits::select_on_container_copy_construction (fetch_allocator (other)))
      { }

      constexpr explicit
      allocator_interface (const alloc_t& alloc)
        : alloc_base (alloc)
      { }

      template <typename From, typename To>
      struct is_memcpyable_integral
      {
        using from = underlying_if_enum_t<From>;
        using to   = underlying_if_enum_t<To>;

        static constexpr
        bool
        value = (sizeof(from) == sizeof(to))
            &&  (std::is_same<bool, from>::value == std::is_same<bool, to>::value)
            &&  std::is_integral<from>::value
            &&  std::is_integral<to>::value;
      };

      template <typename From, typename To>
      struct is_convertible_pointer
        : bool_constant<std::is_pointer<From>::value
                    &&  std::is_pointer<To>::value
                    &&  std::is_convertible<From, To>::value>
      { };

      // memcpyable assignment
      template <typename QualifiedFrom, typename QualifiedTo>
      struct is_memcpyable_impl
      {
        static_assert (! std::is_reference<QualifiedTo>::value,
                       "QualifiedTo must not be a reference.");

        using from = typename std::remove_reference<
          typename std::remove_cv<QualifiedFrom>::type>::type;

        using to = typename std::remove_cv<QualifiedTo>::type;

        static constexpr
        bool
        value = std::is_trivially_assignable<QualifiedTo&, QualifiedFrom>::value
            &&  std::is_trivially_copyable<to>::value
            &&  (  std::is_same<typename std::remove_cv<from>::type, to>::value
               ||  is_memcpyable_integral<from, to>::value
               ||  is_convertible_pointer<from, to>::value);
      };

      template <typename ...Args>
      struct is_memcpyable
        : std::false_type
      { };

      template <typename U>
      struct is_memcpyable<U>
        : is_memcpyable_impl<U, value_t>
      { };

      // memcpyable construction
      template <typename QualifiedFrom, typename QualifiedTo>
      struct is_uninitialized_memcpyable_impl
      {
        static_assert (! std::is_reference<QualifiedTo>::value,
                       "QualifiedTo must not be a reference.");

        using from = typename std::remove_reference<
          typename std::remove_cv<QualifiedFrom>::type>::type;

        using to = typename std::remove_cv<QualifiedTo>::type;

        static constexpr
        bool
        value = std::is_trivially_constructible<QualifiedTo, QualifiedFrom>::value
            &&  std::is_trivially_copyable<to>::value
            &&  (  std::is_same<typename std::remove_cv<from>::type, to>::value
               ||  is_memcpyable_integral<from, to>::value
               ||  is_convertible_pointer<from, to>::value)
            &&  (! must_use_alloc_construct<QualifiedTo>::value
               &&! must_use_alloc_destroy_v);
      };

      template <typename ...Args>
      struct is_uninitialized_memcpyable
        : std::false_type
      { };

      template <typename U>
      struct is_uninitialized_memcpyable<U>
        : is_uninitialized_memcpyable_impl<U, value_t>
      { };

      template <typename Iterator>
      struct is_small_vector_iterator
        : std::false_type
      { };

      template <typename ...Ts>
      struct is_small_vector_iterator<small_vector_iterator<Ts...>>
        : std::true_type
      { };

      template <typename InputIt>
      struct is_contiguous_iterator
        : bool_constant<
                std::is_same<InputIt, ptr>::value
            ||  std::is_same<InputIt, cptr>::value
            ||  is_small_vector_iterator<InputIt>::value
#ifdef GCH_LIB_CONCEPTS
            ||  std::contiguous_iterator<InputIt>
#endif
#ifdef GCH_STDLIB_INTEROP
            ||  std::is_same<InputIt, typename std::array<value_t>::iterator>::value
            ||  std::is_same<InputIt, typename std::array<value_t>::const_iterator>::value
            ||  (! std::is_same<value_t, bool>
               &&  (  std::is_same<InputIt, typename std::vector<value_t>::iterator>::value
                  ||  std::is_same<InputIt, typename std::vector<value_t>::const_iterator>::value)
                )
            ||  std::is_same<InputIt,
                  decltype (std::begin (std::declval<std::valarray<value_t>&> ()))>::value
            ||  std::is_same<InputIt,
                  decltype (std::begin (std::declval<const std::valarray<value_t>&> ()))>::value
#endif
            >
      { };

      template <typename InputIt, typename V = value_t>
      struct is_memcpyable_iterator
        : bool_constant<is_memcpyable<decltype (*std::declval<InputIt> ()), V>::value
                    &&  is_contiguous_iterator<InputIt>::value>
      { };

      // unwrap move_iterators
      template <typename U, typename V>
      struct is_memcpyable_iterator<std::move_iterator<U>, V>
        : is_memcpyable_iterator<U, V>
      { };

      template <typename InputIt, typename V = value_t>
      struct is_uninitialized_memcpyable_iterator
        : bool_constant<is_uninitialized_memcpyable<decltype (*std::declval<InputIt> ()), V>::value
                    &&  is_contiguous_iterator<InputIt>::value>
      { };

      // unwrap move_iterators
      template <typename U, typename V>
      struct is_uninitialized_memcpyable_iterator<std::move_iterator<U>, V>
        : is_uninitialized_memcpyable_iterator<U, V>
      { };

      static constexpr
      value_t *
      to_address (value_t *p) noexcept
      {
        static_assert(! std::is_function<value_t>::value, "value_t is a function pointer.");
        return p;
      }

      static constexpr
      const value_t *
      to_address (const value_t *p) noexcept
      {
        static_assert(! std::is_function<value_t>::value, "value_t is a function pointer.");
        return p;
      }

      template <typename P = ptr,
        typename std::enable_if<! std::is_convertible<P, const value_t*>::value
                              &&  has_ptr_traits_to_address<P>::value>::type * = nullptr>
      static constexpr
      value_t *
      to_address (ptr p) noexcept
      {
        return std::pointer_traits<ptr>::to_address (p);
      }

      template <typename P = ptr,
        typename std::enable_if<! std::is_convertible<P, const value_t*>::value
                              &&  has_ptr_traits_to_address<P>::value>::type * = nullptr>
      static constexpr
      const value_t *
      to_address (cptr p) noexcept
      {
        return std::pointer_traits<cptr>::to_address (p);
      }

      template <typename Pointer,
        typename std::enable_if<! std::is_convertible<Pointer, const value_t*>::value
                              &&! has_ptr_traits_to_address<Pointer>::value>::type * = nullptr>
      static constexpr
      auto
      to_address (Pointer p) noexcept
        -> decltype (to_address (p.operator-> ()))
      {
        return to_address (p.operator-> ());
      }

      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      size_ty
      internal_range_length (cptr first, cptr last) noexcept
      {
        // guaranteed to be leq max size_ty
        return static_cast<size_ty> (std::distance (first, last));
      }

      template <typename Iterator,
                typename Integer,
                typename IteratorDiffT = typename std::iterator_traits<Iterator>::difference_type>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      Iterator
      unchecked_next (Iterator pos, Integer n) noexcept
      {
        return std::next (pos, static_cast<IteratorDiffT> (n));
      }

      template <typename Iterator,
                typename Integer,
                typename IteratorDiffT = typename std::iterator_traits<Iterator>::difference_type>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      Iterator
      unchecked_prev (Iterator pos, Integer n) noexcept
      {
        return std::prev (pos, static_cast<IteratorDiffT> (n));
      }

      template <typename Iterator,
                typename Integer,
                typename IteratorDiffT = typename std::iterator_traits<Iterator>::difference_type>
      static GCH_CPP17_CONSTEXPR
      void
      unchecked_advance (Iterator pos, Integer n) noexcept
      {
        return std::advance (pos, static_cast<IteratorDiffT> (n));
      }

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      size_ty
      get_max_size (void) const noexcept
      {
        // protected from max/min macros
        return (std::min) (static_cast<size_ty> (alloc_traits::max_size (fetch_allocator (*this))),
                           static_cast<size_ty> ((std::numeric_limits<difference_type>::max) ()));
      }

      GCH_NODISCARD GCH_CPP20_CONSTEXPR
      ptr
      allocate (size_ty n)
      {
        return alloc_traits::allocate (fetch_allocator (*this), n);
      }

      GCH_NODISCARD GCH_CPP20_CONSTEXPR
      ptr
      allocate_with_hint (size_ty n, cptr hint)
      {
        return alloc_traits::allocate (fetch_allocator (*this), n, hint);
      }

      GCH_CPP20_CONSTEXPR
      void
      deallocate (ptr p, size_ty n)
      {
        alloc_traits::deallocate (fetch_allocator (*this), to_address (p), n);
      }

      template <typename U,
                typename std::enable_if<is_uninitialized_memcpyable<U>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void
      construct (ptr p, U&& val) noexcept
      {
#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
        if (std::is_constant_evaluated ())
        {
          alloc_traits::construct (fetch_allocator (*this), to_address (p), std::forward<U> (val));
          return;
        }
#endif
        std::memcpy (to_address (p), &val, sizeof (value_t));
      }

      // basically alloc_traits::construct
      // all this is so we can replicate C++20 behavior in the other overload
      template <typename ...Args,
        typename std::enable_if<(  sizeof...(Args) != 1
                               ||! is_uninitialized_memcpyable<Args...>::value)
                            &&  has_alloc_construct<Args...>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void
      construct (ptr p, Args&&... args)
        noexcept (noexcept (alloc_traits::construct (std::declval<alloc_t&> (),
                                                     std::declval<value_t *> (),
                                                     std::forward<Args> (args)...)))
      {
        alloc_traits::construct (fetch_allocator (*this), to_address (p),
                                 std::forward<Args> (args)...);
      }

      template <typename ...Args,
        typename std::enable_if<(  sizeof...(Args) != 1
                               ||! is_uninitialized_memcpyable<Args...>::value)
                            &&! has_alloc_construct<Args...>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void
      construct (ptr p, Args&&... args)
        noexcept (noexcept (::new (std::declval<void *> ()) value_t (std::declval<Args> ()...)))
      {
        construct_at (to_address (p), std::forward<Args> (args)...);
      }

      template <typename V = value_t,
                typename std::enable_if<std::is_trivially_destructible<V>::value
                                    &&! must_use_alloc_destroy_v>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void
      destroy (ptr) const noexcept { }

      template <typename A = alloc_t,
                typename std::enable_if<! (  std::is_trivially_destructible<value_t>::value
                                         &&! must_use_alloc_destroy_v)
                                      &&  has_alloc_destroy<A>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void
      destroy (ptr p) noexcept
      {
        alloc_traits::destroy (fetch_allocator (*this), to_address (p));
      }

      // defined so we match C++20 behavior in all cases.
      template <typename A = alloc_t,
                typename std::enable_if<! (  std::is_trivially_destructible<value_t>::value
                                         &&! must_use_alloc_destroy_v)
                                      &&! has_alloc_destroy<A>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void
      destroy (ptr p) noexcept
      {
        destroy_at (to_address (p));
      }

      template <typename V = value_t,
                typename std::enable_if<std::is_trivially_destructible<V>::value
                                    &&! must_use_alloc_destroy_v>::type * = nullptr>
      GCH_CPP14_CONSTEXPR
      void
      destroy_range (ptr, ptr) const noexcept { }

      template <typename V = value_t,
                typename std::enable_if<! (  std::is_trivially_destructible<V>::value
                                         &&! must_use_alloc_destroy_v)>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void
      destroy_range (ptr first, ptr last) noexcept
      {
        for (; first != last; ++first)
          destroy (first);
      }

      // allowed if trivially copyable and we use the standard allocator
      // and InputIt is a contiguous iterator
      template <typename ForwardIt,
                typename std::enable_if<
                  is_uninitialized_memcpyable_iterator<ForwardIt>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_copy (ForwardIt first, ForwardIt last, ptr dest) noexcept
      {
        static_assert (std::is_constructible<value_t, decltype (*first)>::value,
                       "`value_type` must be copy constructible.");

#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
        if (std::is_constant_evaluated ())
          return default_uninitialized_copy (first, last, dest);
#endif

        const size_ty num_copy = external_range_length (first, last);
        if (num_copy != 0)
          std::memcpy (to_address (dest), to_address (first), num_copy * sizeof (value_t));
        return unchecked_next (dest, num_copy);
      }

      template <typename InputIt,
                typename std::enable_if<
                  ! is_uninitialized_memcpyable_iterator<InputIt>::value, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_copy (InputIt first, InputIt last, ptr d_first)
      {
        return default_uninitialized_copy (first, last, d_first);
      }

      template <typename InputIt>
      GCH_CPP20_CONSTEXPR
      ptr
      default_uninitialized_copy (InputIt first, InputIt last, ptr d_first)
      {
        ptr d_last = d_first;
        try
        {
          for (; first != last; ++first, static_cast<void> (++d_last))
            construct (d_last, *first);
          return d_last;
        }
        catch (...)
        {
          destroy_range (d_first, d_last);
          throw;
        }
      }

      template <typename V = value_t,
        typename std::enable_if<std::is_trivially_constructible<V>::value
                            &&! must_use_alloc_construct<>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_value_construct (ptr first, ptr last)
      {
#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
        if (std::is_constant_evaluated ())
          return default_uninitialized_value_construct (first, last);
#endif

        std::fill (first, last, value_t ());
        return last;
      }

      template <typename V = value_t,
        typename std::enable_if<! (  std::is_trivially_destructible<V>::value
                                 &&! must_use_alloc_construct<>::value)>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_value_construct (ptr first, ptr last)
      {
        return default_uninitialized_value_construct (first, last);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      default_uninitialized_value_construct (ptr first, ptr last)
      {
        ptr curr = first;
        try
        {
          for (; curr != last; ++curr)
            construct (curr);
          return curr;
        }
        catch (...)
        {
          destroy_range (first, curr);
          throw;
        }
      }

      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_fill (ptr first, ptr last, const value_t& val)
      {
        ptr curr = first;
        try
        {
          for (; curr != last; ++curr)
            construct (curr, val);
          return curr;
        }
        catch (...)
        {
          destroy_range (first, curr);
          throw;
        }
      }

      template <typename InputIt,
                typename std::enable_if<
                  is_memcpyable_iterator<InputIt>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      InputIt
      copy_n_return_in (InputIt first, size_ty count, ptr dest) noexcept
      {
        if (count != 0)
          std::memcpy (to_address (dest), to_address (first), count * sizeof (value_t));
        // note: unsafe cast should be proven safe in small_vector_base
        return std::next (first, static_cast<diff_ty> (count));
      }

      template <typename InputIt,
                typename std::enable_if<
                    ! is_memcpyable_iterator<InputIt>::value
                  &&  std::is_base_of<std::random_access_iterator_tag,
                        typename std::iterator_traits<InputIt>::iterator_category>::value
                >::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      InputIt
      copy_n_return_in (InputIt first, size_ty count, ptr dest)
      {
        std::copy_n (first, count, dest);
        // note: unsafe cast should be proven safe in small_vector_base
        return std::next (first, static_cast<diff_ty> (count));
      }

      template <typename InputIt,
                typename std::enable_if<
                    ! is_memcpyable_iterator<InputIt>::value
                  &&! std::is_base_of<std::random_access_iterator_tag,
                        typename std::iterator_traits<InputIt>::iterator_category>::value
                >::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      InputIt
      copy_n_return_in (InputIt first, size_ty count, ptr dest)
      {
        for (; count != 0; --count)
          *dest++ = *first++;
        return first;
      }

      template <typename V = value_t,
                typename std::enable_if<is_memcpyable<V>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr
      move_left (ptr first, ptr last, ptr d_first)
      {
        // shift initialized elements to the left
        // n should not be 0
        const size_ty num_moved = internal_range_length (first, last);
        if (num_moved != 0)
          std::memmove (to_address (d_first), to_address (first), num_moved * sizeof (value_t));
        return unchecked_next (d_first, num_moved);
      }

      template <typename V = value_t,
                typename std::enable_if<! is_memcpyable<V>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr
      move_left (ptr first, ptr last, ptr d_first)
      {
        // shift initialized elements to the left
        // n should not be 0
        return std::move (first, last, d_first);
      }

      template <typename V = value_t,
                typename std::enable_if<is_memcpyable<V>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr
      move_right (ptr first, ptr last, ptr d_last)
      {
        // move initialized elements to the right
        // n should not be 0
        const size_ty num_moved = internal_range_length (first, last);
        const ptr     dest      = unchecked_prev (d_last, num_moved);
        if (num_moved != 0)
          std::memmove (to_address (dest), to_address (first), num_moved * sizeof (value_t));
        return dest;
      }

      template <typename V = value_t,
                typename std::enable_if<! is_memcpyable<V>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr
      move_right (ptr first, ptr last, ptr d_last)
      {
        // move initialized elements to the right
        // n should not be 0
        return std::move_backward (first, last, d_last);
      }

      static constexpr
      alloc_t&
      fetch_allocator (allocator_interface& self) noexcept
      {
        return alloc_base::get (self);
      }

      static constexpr
      const alloc_t&
      fetch_allocator (const allocator_interface& self) noexcept
      {
        return alloc_base::get (self);
      }

    private:
      template <typename U = value_t,
                typename std::enable_if<! std::is_array<U>::value>::type * = nullptr>
      static GCH_CPP20_CONSTEXPR
      void
      destroy_at (value_t *p) noexcept
      {
        p->~value_t ();
      }

      // replicate C++20 behavior
      template <typename U = value_t,
                typename std::enable_if<std::is_array<U>::value>::type * = nullptr>
      static GCH_CPP20_CONSTEXPR
      void
      destroy_at (value_t *p) noexcept
      {
        for (auto& e : *p)
          destroy_at (std::addressof (e));
      }

      template <typename ...Args>
      static GCH_CPP20_CONSTEXPR
      auto
      construct_at (value_t *p, Args&&... args)
        noexcept (noexcept (::new (std::declval<void *> ()) value_t (std::declval<Args> ()...)))
        -> decltype(::new (std::declval<void *> ()) value_t (std::declval<Args> ()...))
      {
#if defined (GCH_LIB_IS_CONSTANT_EVALUATED) && defined (GCH_LIB_CONSTEXPR_MEMORY)
        // because GCC only allows placement new to be constexpr inside std::construct_at
        if (std::is_constant_evaluated ())
          return std::construct_at (p, std::forward<Args> (args)...);
#endif
        return ::new (const_cast<void *> (
          static_cast<const volatile void *> (p))) value_t (std::forward<Args>(args)...);
      }
    };

    template <typename Pointer, typename SizeT>
    class small_vector_data_base
    {
    public:
      using ptr     = Pointer;
      using size_ty = SizeT;

      small_vector_data_base            (void)                              = default;
      small_vector_data_base            (const small_vector_data_base&)     = default;
      small_vector_data_base            (small_vector_data_base&&) noexcept = default;
      small_vector_data_base& operator= (const small_vector_data_base&)     = default;
      small_vector_data_base& operator= (small_vector_data_base&&) noexcept = default;
      ~small_vector_data_base           (void)                              = default;

      constexpr ptr     data_ptr (void) const noexcept { return m_data_ptr; }
      constexpr size_ty capacity (void) const noexcept { return m_capacity; }
      constexpr size_ty size     (void) const noexcept { return m_size; }

      GCH_CPP20_CONSTEXPR void set_data_ptr (ptr     data_ptr) noexcept { m_data_ptr = data_ptr; }
      GCH_CPP20_CONSTEXPR void set_capacity (size_ty capacity) noexcept { m_capacity = capacity; }
      GCH_CPP20_CONSTEXPR void set_size     (size_ty size)     noexcept { m_size     = size;     }

      GCH_CPP20_CONSTEXPR
      void
      set (ptr data_ptr, size_ty capacity, size_ty size)
      {
        m_data_ptr = data_ptr;
        m_capacity = capacity;
        m_size     = size;
      }

      GCH_CPP20_CONSTEXPR
      void
      swap_data_ptr (small_vector_data_base& other) noexcept
      {
        using std::swap;
        swap (m_data_ptr, other.m_data_ptr);
      }

      GCH_CPP20_CONSTEXPR
      void
      swap_capacity (small_vector_data_base& other) noexcept
      {
        using std::swap;
        swap (m_capacity, other.m_capacity);
      }

      GCH_CPP20_CONSTEXPR
      void
      swap_size (small_vector_data_base& other) noexcept
      {
        using std::swap;
        swap (m_size, other.m_size);
      }

      GCH_CPP20_CONSTEXPR
      void
      swap (small_vector_data_base& other) noexcept
      {
        using std::swap;
        swap (m_data_ptr, other.m_data_ptr);
        swap (m_capacity, other.m_capacity);
        swap (m_size,     other.m_size);
      }

    private:
      ptr     m_data_ptr;
      size_ty m_capacity;
      size_ty m_size;
    };

    template <typename Pointer, typename SizeT, typename T, unsigned InlineCapacity>
    class small_vector_data
      : public small_vector_data_base<Pointer, SizeT>
    {
    public:
      using value_t = T;

      small_vector_data            (void)                         = default;
      small_vector_data            (const small_vector_data&)     = delete;
      small_vector_data            (small_vector_data&&) noexcept = delete;
      small_vector_data& operator= (const small_vector_data&)     = delete;
      small_vector_data& operator= (small_vector_data&&) noexcept = delete;
      ~small_vector_data           (void)                         = default;

      GCH_CPP14_CONSTEXPR
      value_t *
      storage (void) noexcept
      {
        return m_storage.get_inline_ptr ();
      }

      constexpr
      const value_t *
      storage (void) const noexcept
      {
        return m_storage.get_inline_ptr ();
      }

    private:
      inline_storage<value_t, InlineCapacity> m_storage;
    };

    template <typename Pointer, typename SizeT, typename T>
    class GCH_EMPTY_BASE small_vector_data<Pointer, SizeT, T, 0>
      : public  small_vector_data_base<Pointer, SizeT>,
        private inline_storage<T, 0>
    {
      using base = inline_storage<T, 0>;

    public:
      using value_t = T;

      small_vector_data            (void)                         = default;
      small_vector_data            (const small_vector_data&)     = delete;
      small_vector_data            (small_vector_data&&) noexcept = delete;
      small_vector_data& operator= (const small_vector_data&)     = delete;
      small_vector_data& operator= (small_vector_data&&) noexcept = delete;
      ~small_vector_data           (void)                         = default;

      GCH_CPP14_CONSTEXPR
      value_t *
      storage (void) noexcept
      {
        return base::get_inline_ptr ();
      }

      constexpr
      const value_t *
      storage (void) const noexcept
      {
        return base::get_inline_ptr ();
      }
    };

    template <typename Allocator, unsigned InlineCapacity>
    class small_vector_base
      : public allocator_interface<Allocator>
    {
    public:
      using size_type       = typename allocator_interface<Allocator>::size_type;
      using difference_type = typename allocator_interface<Allocator>::difference_type;

    protected:
      using alloc_interface = allocator_interface<Allocator>;
      using alloc_t         = Allocator;

      using value_t         = typename alloc_interface::value_t;
      using ptr             = typename alloc_interface::ptr;
      using cptr            = typename alloc_interface::cptr;
      using size_ty         = typename alloc_interface::size_ty;
      using diff_ty         = typename alloc_interface::diff_ty;

      using alloc_interface::unchecked_next;
      using alloc_interface::unchecked_prev;
      using alloc_interface::unchecked_advance;
      using alloc_interface::get_max_size;
      using alloc_interface::deallocate;
      using alloc_interface::construct;
      using alloc_interface::destroy;
      using alloc_interface::destroy_range;
      using alloc_interface::uninitialized_copy;
      using alloc_interface::uninitialized_value_construct;
      using alloc_interface::uninitialized_fill;
      using alloc_interface::copy_n_return_in;
      using alloc_interface::fetch_allocator;
      using alloc_interface::to_address;
      using alloc_interface::internal_range_length;
      using alloc_interface::move_left;
      using alloc_interface::move_right;

    private:
      using small_vector_type = small_vector<value_t, InlineCapacity, alloc_t>;

      template <typename ...>
      using void_t = void;

      template <bool B>
      using bool_constant = std::integral_constant<bool, B>;

      template <typename Void, typename AI, typename V, typename ...Args>
      struct is_emplace_constructible_impl
        : std::false_type
      {
        using nothrow = std::false_type;
      };

      template <typename AI, typename V, typename ...Args>
      struct is_emplace_constructible_impl<
            void_t<decltype (std::declval<AI&> ().construct (std::declval<V *> (),
                                                             std::declval<Args> ()...))>,
            AI, V, Args...>
        : std::true_type
      {
        using nothrow =
          bool_constant<noexcept (std::declval<AI&> ().construct (std::declval<V *> (),
                                                                  std::declval<Args> ()...))>;
      };

      template <typename ...Args>
      struct is_emplace_constructible
        : is_emplace_constructible_impl<void, alloc_interface, value_t, Args...>
      { };

      template <typename ...Args>
      struct is_nothrow_emplace_constructible
        : is_emplace_constructible_impl<void, alloc_interface, value_t, Args...>::nothrow
      { };

      template <typename AI>
      struct is_move_insertable
        : is_emplace_constructible<value_t&&>
      { };

      template <typename AI>
      struct is_nothrow_move_insertable
        : is_nothrow_emplace_constructible<value_t&&>
      { };

      template <typename AI>
      struct is_copy_insertable
        : std::integral_constant<bool, is_move_insertable<AI>::value
                                   &&  is_emplace_constructible<value_t&>::value
                                   &&  is_emplace_constructible<const value_t&>::value>
      { };



      template <typename AI>
      struct is_nothrow_copy_insertable
        : std::integral_constant<bool, is_move_insertable<AI>::value
                                   &&  is_nothrow_emplace_constructible<value_t&>::value
                                   &&  is_nothrow_emplace_constructible<const value_t&>::value>
      { };

      template <typename AI, typename Enable = void>
      struct is_eraseable
        : std::false_type
      { };

      template <typename AI>
      struct is_eraseable<AI,
            void_t<decltype (std::declval<AI&> ().destroy (std::declval<value_t *> ()))>>
        : std::true_type
      { };

      template <typename V>
      struct relocate_with_move
        : bool_constant<std::is_nothrow_move_constructible<V>::value
                    ||! std::is_copy_constructible<V>::value>
      { };

      static constexpr
      bool
      is_move_insertable_v = is_move_insertable<alloc_t>::value;

      static constexpr
      bool
      is_nothrow_move_insertable_v = is_nothrow_move_insertable<alloc_t>::value;

      static constexpr
      bool
      is_copy_insertable_v = is_copy_insertable<alloc_t>::value;

      static constexpr
      bool
      is_nothrow_copy_insertable_v = is_nothrow_copy_insertable<alloc_t>::value;

      static constexpr
      bool
      is_eraseable_v = is_eraseable<alloc_t>::value;

      static constexpr
      size_ty
      constexpr_inline_capacity = InlineCapacity + 1;

    public:
      GCH_NORETURN
      static GCH_CPP20_CONSTEXPR
      void
      throw_overflow_error (void)
      {
        throw std::overflow_error ("The requested conversion would overflow.");
      }

      GCH_NORETURN
      static GCH_CPP20_CONSTEXPR
      void
      throw_index_error (void)
      {
        throw std::out_of_range ("The requested index was out of range.");
      }

      GCH_NORETURN
      static GCH_CPP20_CONSTEXPR
      void
      throw_range_length_error (void)
      {
        throw std::length_error ("The specified range is too long.");
      }

      GCH_NORETURN
      static GCH_CPP20_CONSTEXPR
      void
      throw_increment_error (void)
      {
        throw std::domain_error ("The requested increment was outside of the allowed range.");
      }

      GCH_NORETURN
      static GCH_CPP20_CONSTEXPR
      void
      throw_allocation_size_error (void)
      {
        throw std::length_error ("The required allocation exceeds the maximum size.");
      }

      GCH_NODISCARD
      GCH_CPP14_CONSTEXPR
      ptr
      ptr_cast (const small_vector_iterator<cptr, diff_ty>& it) noexcept
      {
        return std::next (begin_ptr (), it.base () - begin_ptr ());
      }

      GCH_NODISCARD
      static constexpr
      ptr
      ptr_cast (const small_vector_iterator<ptr, diff_ty>& it) noexcept
      {
        return it.base ();
      }

      template <typename Integer>
      GCH_NODISCARD
      static constexpr
      std::size_t
      numeric_max (void) noexcept
      {
        static_assert (0 <= (std::numeric_limits<Integer>::max) (), "Integer is nonpositive.");
        return static_cast<std::size_t> ((std::numeric_limits<Integer>::max) ());
      }

      template <typename To,
                typename From,
        typename std::enable_if<(numeric_max<To> () < numeric_max<From> ()),
                                bool>::type = true>
      GCH_NODISCARD
      static GCH_CPP14_CONSTEXPR
      To
      checked_numeric_cast (const From n) noexcept
      {
#ifndef NDEBUG
        if (numeric_max<To> () < n)
          throw_overflow_error ();
#endif
        return static_cast<To> (n);
      }

      template <typename To,
                typename From,
                typename std::enable_if<! (numeric_max<To> () < numeric_max<From> ()),
                                        bool>::type = false>
      GCH_NODISCARD
      static GCH_CPP14_CONSTEXPR
      To
      checked_numeric_cast (const From n) noexcept
      {
        return static_cast<To> (n);
      }

      template <typename Iterator,
                typename UInteger,
                typename IteratorDiffT = typename std::iterator_traits<Iterator>::difference_type,
                typename std::enable_if<std::is_unsigned<UInteger>::value
                                    &&  (numeric_max<IteratorDiffT> () < numeric_max<UInteger> ()),
                                        bool>::type = true>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      Iterator
      checked_next (Iterator pos, UInteger n) noexcept
      {
#ifndef NDEBUG
        if (numeric_max<IteratorDiffT> () < static_cast<std::size_t> (n))
          throw_increment_error ();
#endif
        return std::next (pos, static_cast<IteratorDiffT> (n));
      }

      template <typename Iterator,
                typename UInteger,
                typename IteratorDiffT = typename std::iterator_traits<Iterator>::difference_type,
                typename std::enable_if<std::is_unsigned<UInteger>::value
                                    &&! (numeric_max<IteratorDiffT> () < numeric_max<UInteger> ()),
                                        bool>::type = false>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      Iterator
      checked_next (Iterator pos, UInteger n) noexcept
      {
        return std::next (pos, static_cast<IteratorDiffT> (n));
      }

      template <typename Iterator,
                typename UInteger,
                typename IteratorDiffT = typename std::iterator_traits<Iterator>::difference_type,
                typename std::enable_if<std::is_unsigned<UInteger>::value
                                    &&  (numeric_max<IteratorDiffT> () < numeric_max<UInteger> ()),
                                        bool>::type = true>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      Iterator
      checked_prev (Iterator pos, UInteger n) noexcept
      {
#ifndef NDEBUG
        if (numeric_max<IteratorDiffT> () < static_cast<std::size_t> (n))
          throw_increment_error ();
#endif
        return std::prev (pos, static_cast<IteratorDiffT> (n));
      }

      template <typename Iterator,
                typename UInteger,
                typename IteratorDiffT = typename std::iterator_traits<Iterator>::difference_type,
                typename std::enable_if<std::is_unsigned<UInteger>::value
                                    &&! (numeric_max<IteratorDiffT> () < numeric_max<UInteger> ()),
                                        bool>::type = false>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      Iterator
      checked_prev (Iterator pos, UInteger n) noexcept
      {
        return std::prev (pos, static_cast<IteratorDiffT> (n));
      }

    private:
      template <typename RandomIt>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      size_ty
      external_range_length_impl (RandomIt first, RandomIt last,
                                  std::random_access_iterator_tag) noexcept
      {
        GCH_ASSERT (first <= last && "Invalid range.");
        const auto len = static_cast<std::size_t> (std::distance (first, last));
#ifndef NDEBUG
        if ((std::numeric_limits<size_ty>::max) () < len)
          throw_range_length_error ();
#endif
        return static_cast<size_ty> (len);
      }

      template <typename ForwardIt>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      size_ty
      external_range_length_impl (ForwardIt first, ForwardIt last,
                                  std::forward_iterator_tag) noexcept
      {
        const auto len = static_cast<std::size_t> (std::distance (first, last));
#ifndef NDEBUG
        if ((std::numeric_limits<size_ty>::max) () < len)
          throw_range_length_error ();
#endif
        return static_cast<size_ty> (len);
      }

      template <typename ForwardIt,
                typename ItDiffT = typename std::iterator_traits<ForwardIt>::difference_type,
                typename std::enable_if<(numeric_max<size_ty> () < numeric_max<ItDiffT> ()),
                                        bool>::type = true>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      size_ty
      external_range_length (ForwardIt first, ForwardIt last) noexcept
      {
        using iterator_cat = typename std::iterator_traits<ForwardIt>::iterator_category;
        return external_range_length_impl (first, last, iterator_cat { });
      }

      template <typename ForwardIt,
                typename ItDiffT = typename std::iterator_traits<ForwardIt>::difference_type,
                typename std::enable_if<! (numeric_max<size_ty> () < numeric_max<ItDiffT> ()),
                                        bool>::type = false>
      GCH_NODISCARD
      static GCH_CPP17_CONSTEXPR
      size_ty
      external_range_length (ForwardIt first, ForwardIt last) noexcept
      {
        return static_cast<size_ty> (std::distance (first, last));
      }

      class temporary
      {
      public:
        temporary            (void)                 = delete;
        temporary            (const temporary&)     = delete;
        temporary            (temporary&&) noexcept = delete;
        temporary& operator= (const temporary&)     = delete;
        temporary& operator= (temporary&&) noexcept = delete;
//      ~temporary           (void)                 = impl;

        template <typename ...Args>
        GCH_CPP20_CONSTEXPR explicit
        temporary (alloc_interface& interface, Args&&... args)
          : m_interface (interface)
        {
          m_interface.construct (get_pointer (), std::forward<Args> (args)...);
        }

        GCH_CPP20_CONSTEXPR
        ~temporary (void)
        {
          m_interface.destroy (get_pointer ());
        }

        GCH_CPP20_CONSTEXPR
        value_t&
        get (void) noexcept
        {
          return *get_pointer ();
        }

      private:
        GCH_NODISCARD
        GCH_CPP20_CONSTEXPR
        ptr
        get_pointer (void) noexcept
        {
          return static_cast<ptr> (static_cast<void *> (std::addressof (m_data)));
        }

        alloc_interface& m_interface;
        typename std::aligned_storage<sizeof (value_t), alignof (value_t)>::type m_data;
      };

      GCH_CPP20_CONSTEXPR
      void
      set_data_ptr (ptr data_ptr) noexcept
      {
        m_data.set_data_ptr (data_ptr);
      }

      GCH_CPP20_CONSTEXPR
      void
      set_capacity (size_ty capacity) noexcept
      {
        m_data.set_capacity (capacity);
      }

      GCH_CPP20_CONSTEXPR
      void
      set_size (size_ty size) noexcept
      {
        m_data.set_size (size);
      }

      GCH_CPP20_CONSTEXPR
      void
      set_data (ptr data_ptr, size_ty capacity, size_ty size) noexcept
      {
        m_data.set (data_ptr, capacity, size);
      }

      GCH_CPP20_CONSTEXPR
      void
      swap_data_ptr (small_vector_base& other) noexcept
      {
        m_data.swap_data (other.m_data);
      }

      GCH_CPP20_CONSTEXPR
      void
      swap_capacity (small_vector_base& other) noexcept
      {
        m_data.swap_capacity (other.m_data);
      }

      GCH_CPP20_CONSTEXPR
      void
      swap_size (small_vector_base& other) noexcept
      {
        m_data.swap_size (other.m_data);
      }

      GCH_CPP20_CONSTEXPR
      void
      swap_data (small_vector_base& other) noexcept
      {
        m_data.swap (other.m_data);
      }

      GCH_CPP20_CONSTEXPR
      void
      increase_size (size_ty n) noexcept
      {
        m_data.set_size (get_size () + n);
      }

      GCH_CPP20_CONSTEXPR
      void
      decrease_size (size_ty n) noexcept
      {
        m_data.set_size (get_size () - n);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      checked_allocate (size_ty n)
      {
        if (get_max_size () < n)
          throw_allocation_size_error ();
        return unchecked_allocate (n);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      unchecked_allocate (size_ty n)
      {
        GCH_ASSERT (InlineCapacity < n
                &&  "Allocated capacity should be greater than InlineCapacity.");
        return alloc_interface::allocate (n);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      unchecked_allocate (size_ty n, cptr hint)
      {
        GCH_ASSERT (InlineCapacity < n
                &&  "Allocated capacity should be greater than InlineCapacity.");
        return alloc_interface::allocate_with_hint (n, hint);
      }

    public:
      GCH_CPP20_CONSTEXPR
      small_vector_base (void) noexcept
      {
        default_initialize ();
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base (const small_vector_base& other)
        : alloc_interface (other)
      {
        copy_initialize (other);
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base (small_vector_base&& other)
        noexcept (std::is_nothrow_move_constructible<value_t>::value)
        : alloc_interface (std::move (other))
      {
        move_initialize (std::move (other));
      }

      GCH_CPP20_CONSTEXPR explicit
      small_vector_base (const alloc_t& alloc) noexcept
        : alloc_interface (alloc)
      {
        default_initialize ();
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base (const small_vector_base& other, const alloc_t& alloc)
        : alloc_interface (alloc)
      {
        copy_initialize (other);
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base (small_vector_base&& other, const alloc_t& alloc)
        noexcept (std::is_nothrow_move_constructible<value_t>::value)
        : alloc_interface (alloc)
      {
        move_initialize (std::move (other));
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base (size_ty count, const alloc_t& alloc)
        : alloc_interface (alloc)
      {
        if (count <= InlineCapacity)
          initialize_inline_storage ();
        else
        {
          set_data_ptr (checked_allocate (count));
          set_capacity (count);
        }

        try
        {
          uninitialized_value_construct (begin_ptr (), unchecked_next (begin_ptr (), count));
        }
        catch (...)
        {
          if (has_allocation ())
            deallocate (begin_ptr (), get_capacity ());
          throw;
        }
        set_size (count);
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base (size_ty count, const value_t& val, const alloc_t& alloc)
        : alloc_interface (alloc)
      {
        if (count <= InlineCapacity)
          initialize_inline_storage ();
        else
        {
          set_data_ptr (checked_allocate (count));
          set_capacity (count);
        }

        try
        {
          uninitialized_fill (begin_ptr (), unchecked_next (begin_ptr (), count), val);
        }
        catch (...)
        {
          if (has_allocation ())
            deallocate (begin_ptr (), get_capacity ());
          throw;
        }
        set_size (count);
      }

#ifdef GCH_LIB_CONCEPTS
      template <std::input_iterator InputIt>
#else
      template <typename InputIt>
#endif
      GCH_CPP20_CONSTEXPR
      small_vector_base (InputIt first, InputIt last, std::input_iterator_tag,
                         const alloc_t& alloc)
        : small_vector_base (alloc)
      {
        using iterator_cat = typename std::iterator_traits<InputIt>::iterator_category;
        append_range (first, last, iterator_cat { });
      }

#ifdef GCH_LIB_CONCEPTS
      template <std::forward_iterator ForwardIt>
#else
      template <typename ForwardIt>
#endif
      GCH_CPP20_CONSTEXPR
      small_vector_base (ForwardIt first, ForwardIt last, std::forward_iterator_tag,
                         const alloc_t& alloc)
        : alloc_interface (alloc)
      {
        size_t count = external_range_length (first, last);
        if (count <= InlineCapacity)
          initialize_inline_storage ();
        else
        {
          set_data_ptr (unchecked_allocate (count));
          set_capacity (count);
        }

        try
        {
          uninitialized_copy(first, last, begin_ptr ());
        }
        catch (...)
        {
          if (has_allocation ())
            deallocate (begin_ptr (), get_capacity ());
          throw;
        }
        set_size (count);
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base&
      operator= (const small_vector_base&)
        noexcept (std::is_nothrow_copy_constructible<value_t>::value
              &&  std::is_nothrow_copy_assignable<value_t>::value);

      GCH_CPP20_CONSTEXPR
      small_vector_base&
      operator= (small_vector_base&&) noexcept;

      GCH_CPP20_CONSTEXPR
      ~small_vector_base (void) noexcept
      {
        GCH_ASSERT (InlineCapacity <= get_capacity () && "invalid capacity");
        destroy_range (begin_ptr (), end_ptr ());
        if (has_allocation ())
          deallocate (begin_ptr (), get_capacity ());
      }

      GCH_CPP20_CONSTEXPR
      void
      default_initialize (void)
      {
        initialize_inline_storage ();
        set_size (0);
      }

      GCH_CPP20_CONSTEXPR
      void
      copy_initialize (const small_vector_base& other)
      {
        if (! other.has_allocation ())
          set_data_ptr (storage_ptr ());
        else
          set_data_ptr (unchecked_allocate (other.get_capacity (), other.uninitialized_end_ptr ()));
        set_capacity (other.get_capacity ());

        try
        {
          uninitialized_copy (other.begin_ptr (), other.end_ptr (), data_ptr ());
        }
        catch (...)
        {
          if (has_allocation ())
            deallocate (data_ptr (), get_capacity ());
          throw;
        }
        set_size (other.get_size ());
      }

      GCH_CPP20_CONSTEXPR
      void
      move_initialize (small_vector_base&& other)
        noexcept (std::is_nothrow_move_constructible<value_t>::value)
      {
        if (! other.has_allocation ())
        {
          set_data_ptr (storage_ptr ());
          set_capacity (InlineCapacity);
          uninitialized_move (other.begin_ptr (), other.end_ptr (), data_ptr ());
          set_size (other.get_size ());
        }
        else
          set_data (other.data_ptr (), other.get_capacity (), other.get_size ());

        // `other` is reset
        other.default_initialize ();
      }

      GCH_CPP20_CONSTEXPR
      void
      initialize_inline_storage (void)
      {
#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
        if (std::is_constant_evaluated ())
        {
          set_data_ptr (unchecked_allocate (constexpr_inline_capacity));
          set_capacity (constexpr_inline_capacity);
          return;
        }
#endif
        set_data_ptr (storage_ptr ());
        set_capacity (InlineCapacity);
      }

      GCH_CPP20_CONSTEXPR
      void
      assign_copies (size_ty count, const value_t& val)
      {
        if (get_capacity () < count)
        {
          size_ty new_capacity = calculate_new_capacity (count);
          ptr     new_begin    = checked_allocate (new_capacity);

          try
          {
            uninitialized_fill (new_begin, unchecked_next (new_begin, count), val);
          }
          catch (...)
          {
            deallocate (new_begin, new_capacity);
            throw;
          }

          destroy_range (begin_ptr (), end_ptr ());
          deallocate (begin_ptr (), get_capacity ());

          set_data (new_begin, new_capacity, count);
        }
        else if (get_size () < count)
        {
          std::fill (begin_ptr (), end_ptr (), val);
          uninitialized_fill (end_ptr (), unchecked_next (begin_ptr (), count), val);
          set_size (count);
        }
        else
          erase_range (std::fill_n (begin_ptr (), count, val), end_ptr ());
      }

      // just to ensure what we are getting
#ifdef GCH_LIB_CONCEPTS
      template <std::input_iterator InputIt>
#else
      template <typename InputIt>
#endif
      GCH_CPP20_CONSTEXPR
      void
      assign_range (InputIt first, InputIt last, std::input_iterator_tag)
      {
        using iterator_cat = typename std::iterator_traits<InputIt>::iterator_category;

        ptr curr = begin_ptr ();
        while (first != last && curr != end_ptr ())
          *curr++ = *first++;

        if (first == last)
          erase_to_end (curr);
        else
          append_range (first, last, iterator_cat { });
      }

      // just to ensure what we are getting
#ifdef GCH_LIB_CONCEPTS
      template <std::forward_iterator ForwardIt>
#else
      template <typename ForwardIt>
#endif
      GCH_CPP20_CONSTEXPR
      void
      assign_range (ForwardIt first, ForwardIt last, std::forward_iterator_tag)
      {
        const size_ty count = external_range_length (first, last);
        if (get_capacity () < count)
        {
          size_ty new_capacity = calculate_new_capacity (count);
          ptr     new_begin    = checked_allocate (new_capacity);

          try
          {
            uninitialized_copy (first, last, new_begin);
          }
          catch (...)
          {
            deallocate (new_begin, new_capacity);
            throw;
          }

          destroy_range (begin_ptr (), end_ptr ());
          deallocate (begin_ptr (), get_capacity ());

          set_data (new_begin, new_capacity, count);
        }
        else if (get_size () < count)
        {
          ForwardIt pivot = copy_n_return_in (first, get_size (), begin_ptr ());
          uninitialized_copy (pivot, last, end_ptr ());
          set_size (count);
        }
        else
          erase_range (std::copy (first, last, begin_ptr ()), end_ptr ());
      }

      GCH_NODISCARD
      GCH_CPP14_CONSTEXPR
      ptr
      data_ptr (void) noexcept
      {
        return m_data.data_ptr ();
      }

      GCH_NODISCARD
      constexpr
      cptr
      data_ptr (void) const noexcept
      {
        return m_data.data_ptr ();
      }

      GCH_NODISCARD
      constexpr
      size_ty
      get_capacity (void) const noexcept
      {
        return m_data.capacity ();
      }

      GCH_NODISCARD
      constexpr
      size_ty
      get_size (void) const noexcept
      {
        return m_data.size ();
      }

      GCH_NODISCARD
      constexpr
      bool
      is_empty (void) const noexcept
      {
        return get_size () == 0;
      }

      GCH_NODISCARD
      constexpr
      size_ty
      num_uninitialized (void) const noexcept
      {
        return get_capacity () - get_size ();
      }

      GCH_NODISCARD
      GCH_CPP14_CONSTEXPR
      ptr
      begin_ptr (void) noexcept
      {
        return data_ptr ();
      }

      GCH_NODISCARD
      constexpr
      cptr
      begin_ptr (void) const noexcept
      {
        return data_ptr ();
      }

      GCH_NODISCARD
      GCH_CPP14_CONSTEXPR
      ptr
      end_ptr (void) noexcept
      {
        return unchecked_next (begin_ptr (), get_size ());
      }

      GCH_NODISCARD
      constexpr
      cptr
      end_ptr (void) const noexcept
      {
        return unchecked_next (begin_ptr (), get_size ());
      }

      GCH_NODISCARD
      GCH_CPP14_CONSTEXPR
      ptr
      uninitialized_end_ptr (void) noexcept
      {
        return unchecked_next (begin_ptr (), get_capacity ());
      }

      GCH_NODISCARD
      constexpr
      cptr
      uninitialized_end_ptr (void) const noexcept
      {
        return unchecked_next (begin_ptr (), get_capacity ());
      }

      GCH_NODISCARD
      constexpr
      alloc_t
      copy_allocator (void) const noexcept
      {
        return alloc_t (fetch_allocator (*this));
      }

      GCH_NODISCARD
      GCH_CPP14_CONSTEXPR
      ptr
      storage_ptr (void) noexcept
      {
        return m_data.storage ();
      }

      GCH_NODISCARD
      constexpr
      cptr
      storage_ptr (void) const noexcept
      {
        return m_data.storage ();
      }

      GCH_NODISCARD
      constexpr
      bool
      has_allocation (void) const noexcept
      {
        return InlineCapacity < get_capacity ();
      }

      GCH_NODISCARD
      constexpr
      bool
      is_inlinable (void) const noexcept
      {
        return get_size () <= InlineCapacity;
      }

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      size_ty
      calculate_new_capacity (const size_ty new_size) const noexcept
      {
        size_ty capacity = get_capacity ();

        if (get_max_size () - capacity < capacity)
          return get_max_size ();

        // this growth factor actually sucks...
        // size_ty new_capacity = capacity + (capacity / 2);

        size_ty new_capacity = 2 * capacity;
        if (new_capacity < new_size)
          return new_size;
        return new_capacity;
      }

      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_move (ptr first, ptr last, ptr d_first) noexcept
      {
        return uninitialized_copy (std::make_move_iterator (first),
                                   std::make_move_iterator (last),
                                   d_first);
      }

      template <typename V = value_t,
                typename std::enable_if<relocate_with_move<V>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_move_if_noexcept (ptr first, ptr last, ptr dest)
      {
        return uninitialized_move (first, last, dest);
      }

      template <typename V = value_t,
                typename std::enable_if<! relocate_with_move<V>::value, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_move_if_noexcept (ptr first, ptr last, ptr dest)
      {
        return uninitialized_copy (first, last, dest);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      shift_into_uninitialized (ptr pos, size_ty n_shift)
      {
        // shift elements over to the right into uninitialized space
        // returns the start of the new range
        // precondition: shift <= end_ptr () - pos
        if (n_shift == 0)
          return pos;

        ptr original_end = end_ptr ();
        ptr pivot        = unchecked_prev (original_end, n_shift);

        uninitialized_move (pivot, original_end, original_end);
        increase_size (n_shift);
        return move_right (pos, pivot, original_end);
      }

      template <typename ...Args>
      GCH_CPP20_CONSTEXPR
      ptr
      append_element (Args&&... args)
      {
        if (get_size () < get_capacity ())
          return emplace_into_current_end (std::forward<Args> (args)...);
        else
          return emplace_into_reallocation (end_ptr (), std::forward<Args> (args)...);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      append_copies (size_ty count, const value_t& val)
      {
        if (count <= num_uninitialized ())
        {
          const ptr ret = end_ptr ();
          uninitialized_fill (ret, unchecked_next (ret, count), val);
          increase_size (count);
          return ret;
        }
        else
        {
          // reallocate
          if (get_max_size () - get_size () < count)
            throw_allocation_size_error ();

          size_ty original_size = get_size ();
          size_ty new_size      = get_size () + count;

          size_ty new_capacity = calculate_new_capacity (new_size);

          // check is handled by the if-guard
          ptr new_data_ptr = unchecked_allocate (new_capacity, uninitialized_end_ptr ());
          ptr new_last     = unchecked_next (new_data_ptr, original_size);

          try
          {
            new_last = uninitialized_fill (new_last, unchecked_next (new_last, count), val);
            uninitialized_move (begin_ptr (), end_ptr (), new_data_ptr);
          }
          catch (...)
          {
            destroy_range (unchecked_next (new_data_ptr, original_size), new_last);
            deallocate (new_data_ptr, new_capacity);
            throw;
          }

          destroy_range (begin_ptr (), end_ptr ());
          if (has_allocation ())
            deallocate (begin_ptr (), get_capacity ());

          set_data (new_data_ptr, new_capacity, new_size);
          return unchecked_next (new_data_ptr, original_size);
        }
      }

#ifdef GCH_LIB_CONCEPTS
      template <std::input_iterator InputIt>
#else
      template <typename InputIt>
#endif
      GCH_CPP20_CONSTEXPR
      ptr
      append_range (InputIt first, InputIt last, std::input_iterator_tag)
      {
        size_ty original_size = get_size ();
        while (first != last)
          append_element (*first++);
        return unchecked_next (begin_ptr (), original_size);
      }

#ifdef GCH_LIB_CONCEPTS
      template <std::forward_iterator ForwardIt>
#else
      template <typename ForwardIt>
#endif
      GCH_CPP20_CONSTEXPR
      ptr
      append_range (ForwardIt first, ForwardIt last, std::forward_iterator_tag)
      {
        const size_ty num_insert = external_range_length (first, last);
        if (num_insert <= num_uninitialized ())
        {
          ptr ret = end_ptr ();
          uninitialized_copy (first, last, ret);
          increase_size (num_insert);
          return ret;
        }
        else
        {
          // reallocate
          if (get_max_size () - get_size () < num_insert)
            throw_allocation_size_error ();

          size_ty original_size = get_size ();
          size_ty new_size      = get_size () + num_insert;
          size_ty new_capacity  = calculate_new_capacity (new_size);

          // check is handled by the if-guard
          ptr new_data_ptr = unchecked_allocate (new_capacity, uninitialized_end_ptr ());
          ptr new_last     = unchecked_next (new_data_ptr, original_size);

          try
          {
            new_last = uninitialized_copy (first, last, new_last);
            uninitialized_move (begin_ptr (), end_ptr (), new_data_ptr);
          }
          catch (...)
          {
            destroy_range (unchecked_next (new_data_ptr, original_size), new_last);
            deallocate (new_data_ptr, new_capacity);
            throw;
          }

          destroy_range (begin_ptr (), end_ptr ());
          if (has_allocation ())
            deallocate (begin_ptr (), get_capacity ());

          set_data (new_data_ptr, new_capacity, new_size);
          return unchecked_next (new_data_ptr, original_size);
        }
      }

      template <typename ...Args>
      GCH_CPP20_CONSTEXPR
      ptr
      emplace_at (ptr pos, Args&&... args)
      {
        GCH_ASSERT (get_size () <= get_capacity () && "size was greater than capacity");
        if (get_size () < get_capacity ())
          return emplace_into_current (pos, std::forward<Args> (args)...);
        else
          return emplace_into_reallocation (pos, std::forward<Args> (args)...);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      insert_copies (ptr pos, size_ty count, const value_t& val);

#ifdef GCH_LIB_CONCEPTS
      template <std::input_iterator InputIt>
#else
      template <typename InputIt>
#endif
      GCH_CPP20_CONSTEXPR
      ptr
      insert_range (ptr pos, InputIt first, InputIt last, std::input_iterator_tag)
      {
        if (pos == end_ptr ())
        {
          const size_ty pos_offset = internal_range_length (begin_ptr (), pos);
          while (first != last)
            append_element (*first++);
          return unchecked_next (begin_ptr (), pos_offset);
        }
        else if (first != last)
        {
          small_vector_type tmp (first, last, fetch_allocator (*this));

          // send off to the other overload
          using move_iter = std::move_iterator<small_vector_iterator<ptr, diff_ty>>;
          using move_iter_cat = typename std::iterator_traits<move_iter>::iterator_category;

          static_assert (! std::is_same<move_iter_cat, std::input_iterator_tag>::value,
                         "Unexcepted internal iterator category.");

          return insert_range (pos,
                               move_iter { tmp.begin () },
                               move_iter { tmp.end () },
                               move_iter_cat { });
        }
      }

#ifdef GCH_LIB_CONCEPTS
      template <std::forward_iterator ForwardIt>
#else
      template <typename ForwardIt>
#endif
      GCH_CPP20_CONSTEXPR
      ptr
      insert_range (ptr pos, ForwardIt first, ForwardIt last, std::forward_iterator_tag);

      template <typename ...Args>
      GCH_CPP20_CONSTEXPR
      ptr
      emplace_into_current (ptr pos, value_t&& val)
      {
        if (pos == end_ptr ())
          return emplace_into_current_end (std::move (val));

        // in the special case of value_t&& we don't make a copy
        // because behavior is unspecified when it is an internal
        // element
        shift_into_uninitialized (pos, 1);
        *pos = std::move (val);
        return pos;
      }

      template <typename ...Args>
      GCH_CPP20_CONSTEXPR
      ptr
      emplace_into_current (ptr pos, Args&&... args)
      {
        if (pos == end_ptr ())
          return emplace_into_current_end (std::forward<Args> (args)...);

#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
        if (std::is_constant_evaluated ())
        {
          // need to use the heap
          ptr heap_ptr = unchecked_allocate (sizeof (value_t));
          construct (heap_ptr, std::forward<Args> (args)...);

          shift_into_uninitialized (pos, 1);
          *pos = std::move (*heap_ptr);

          destroy (heap_ptr);
          deallocate (heap_ptr, sizeof (value_t));
          return pos;
        }
#endif
        temporary tmp (*this, std::forward<Args> (args)...);
        shift_into_uninitialized (pos, 1);
        *pos = std::move (tmp.get ());
        return pos;
      }

      template <typename ...Args>
      GCH_CPP20_CONSTEXPR
      ptr
      emplace_into_current_end (Args&&... args)
      {
        construct (end_ptr (), std::forward<Args> (args)...);
        increase_size (1);
        return std::prev (end_ptr ());
      }

      template <typename ...Args>
      GCH_CPP20_CONSTEXPR
      ptr
      emplace_into_reallocation (ptr pos, Args&&... args);

      GCH_CPP20_CONSTEXPR
      ptr
      shrink_to_size (void);

      GCH_CPP20_CONSTEXPR
      void
      resize_default (size_ty count)
      {
        if (count == 0)
          erase_all ();

        if (get_size () < count)
        {
          if (count <= num_uninitialized ())
          {
            uninitialized_value_construct (end_ptr (), unchecked_next (begin_ptr (), count));
            set_size (count);
          }
          else
          {
            // reallocate
            if (get_max_size () - get_size () < count)
              throw_allocation_size_error ();

            size_ty original_size = get_size ();
            size_ty new_size      = get_size () + count;
            size_ty new_capacity  = calculate_new_capacity (new_size);

            // check is handled by the if-guard
            ptr new_data_ptr = unchecked_allocate (new_capacity, uninitialized_end_ptr ());
            ptr new_last     = unchecked_next (new_data_ptr, original_size);

            try
            {
              new_last = uninitialized_value_construct (new_last,
                                                        unchecked_next (new_data_ptr, count));
              uninitialized_move_if_noexcept (begin_ptr (), end_ptr (), new_data_ptr);
            }
            catch (...)
            {
              destroy_range (unchecked_next (new_data_ptr, original_size), new_last);
              deallocate (new_data_ptr, new_capacity);
              throw;
            }

            destroy_range (begin_ptr (), end_ptr ());
            if (has_allocation ())
              deallocate (begin_ptr (), get_capacity ());

            set_data (new_data_ptr, new_capacity, new_size);
          }
        }
        else if (count < get_size ())
          erase_range (unchecked_next (begin_ptr (), count), end_ptr ());
      }

      GCH_CPP20_CONSTEXPR
      void
      request_capacity (size_ty request)
      {
        if (request <= get_capacity ())
          return;

        size_ty new_capacity = calculate_new_capacity (request);
        ptr     new_begin    = checked_allocate (new_capacity);

        try
        {
          uninitialized_move_if_noexcept (begin_ptr (), end_ptr (), new_begin);
        }
        catch (...)
        {
          deallocate (new_begin, new_capacity);
          throw;
        }

        destroy_range (begin_ptr (), end_ptr ());
        if (has_allocation ())
          deallocate (begin_ptr (), get_capacity ());

        set_data_ptr (new_begin);
        set_capacity (new_capacity);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      erase_at (ptr pos)
      {
        move_left (std::next (pos), end_ptr (), pos);
        erase_last ();
        return pos;
      }

      GCH_CPP20_CONSTEXPR
      void erase_last (void)
      {
        destroy (std::prev (end_ptr ()));
        decrease_size (1);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      erase_range (ptr first, ptr last)
      {
        if (first != last)
          erase_to_end (move_left (last, end_ptr (), first));
        return first;
      }

      GCH_CPP20_CONSTEXPR
      void erase_to_end (ptr pos)
      {
        GCH_ASSERT (pos <= end_ptr () && "`pos` was in the uninitialized range");
        if (size_ty change = internal_range_length (pos, end_ptr ()))
        {
          destroy_range (pos, end_ptr ());
          decrease_size (change);
        }
      }

      GCH_CPP20_CONSTEXPR
      void
      erase_all (void)
      {
        destroy_range (begin_ptr (), end_ptr ());
        set_size (0);
      }

      template <typename V = value_t>
      GCH_CPP20_CONSTEXPR
      typename std::enable_if<std::is_move_constructible<V>::value
                          &&  std::is_move_assignable<V>::value
#ifdef GCH_LIB_IS_SWAPPABLE
                          &&  std::is_swappable<V>::value
#endif
                              >::type
      swap (small_vector_base& other)
        noexcept ((std::allocator_traits<alloc_t>::propagate_on_container_swap::value
#ifdef GCH_LIB_IS_ALWAYS_EQUAL
               ||  std::allocator_traits<alloc_t>::is_always_equal::value
#endif
                   )
#ifdef GCH_LIB_IS_SWAPPABLE
              &&  std::is_nothrow_swappable<value_t>::value
#else
              &&  detail::small_vector_adl::is_nothrow_swappable<value_t>::value
#endif
              &&  std::is_nothrow_move_constructible<value_t>::value);

    private:
      small_vector_data<ptr, size_type, value_t, InlineCapacity> m_data;
    };

    /* some implementations to dissuade the compiler from inlining */

    // available for CopyInsertable and CopyAssignable
    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    auto
    small_vector_base<Allocator, InlineCapacity>::
    operator= (const small_vector_base& other)
      noexcept (std::is_nothrow_copy_constructible<value_t>::value
            &&  std::is_nothrow_copy_assignable<value_t>::value)
      -> small_vector_base&
    {
      GCH_ASSERT (&other != this
              &&  "`small_vector` private base `small_vector_base` "
                  "should not participate in self-copy-assignment.");

      if (get_capacity () < other.get_size ())
      {
        // reallocate
        size_ty new_capacity = calculate_new_capacity (other.get_size ());
        ptr     new_begin    = unchecked_allocate (new_capacity, other.uninitialized_end_ptr ());

        uninitialized_copy (other.begin_ptr (), other.end_ptr (), new_begin);
        destroy_range (begin_ptr (), end_ptr ());

        if (has_allocation ())
          deallocate (begin_ptr (), get_capacity ());

        set_data_ptr (new_begin);
        set_size     (new_capacity);
      }
      else if (get_size () < other.get_size ())
      {
        // no reallocation, partially in uninitialized space
        std::copy_n (other.begin_ptr (), get_size (), begin_ptr ());
        uninitialized_copy (unchecked_next (other.begin_ptr (), get_size ()),
                            other.end_ptr(),
                            end_ptr ());
      }
      else
        destroy_range (std::copy (other.begin_ptr (), other.end_ptr (), begin_ptr ()), end_ptr ());

      set_size (other.get_size ());
      alloc_interface::operator= (other);
      return *this;
    }

    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    auto
    small_vector_base<Allocator, InlineCapacity>::
    operator= (small_vector_base&& other) noexcept
      -> small_vector_base&
    {
      GCH_ASSERT (&other != this
                    &&  "`small_vector` private base `small_vector_base` "
                        "should not participate in self-copy-assignment.");

      if (! other.has_allocation ())
      {
        // we are guaranteed to have more capacity than `other`
        if (get_size () < other.get_size ())
        {
          std::copy_n (std::make_move_iterator (other.begin_ptr ()), get_size (), begin_ptr ());
          uninitialized_move (unchecked_next (other.begin_ptr (), get_size ()),
                              other.end_ptr (),
                              unchecked_next (begin_ptr (), get_size ()));
        }
        else
        {
          std::copy_n (std::make_move_iterator (other.begin_ptr ()), other.get_size (),
                       begin_ptr ());
          destroy_range (unchecked_next (begin_ptr (), other.get_size ()), end_ptr ());
        }

        // data_ptr and capacity does not change in this case
        set_size (other.get_size ());
      }
      else
      {
        destroy_range (begin_ptr (), end_ptr ());
        if (has_allocation ())
          deallocate (data_ptr (), get_capacity ());
        set_data (other.data_ptr (), other.get_capacity (), other.get_size ());

        // `other` is reset
        other.default_initialize ();
      }

      alloc_interface::operator= (std::move (other));
      return *this;
    }

    template <typename Allocator, unsigned InlineCapacity>
    template <typename ...Args>
    GCH_CPP20_CONSTEXPR
    auto
    small_vector_base<Allocator, InlineCapacity>::
    emplace_into_reallocation (ptr pos, Args&&... args)
      -> ptr
    {
      if (get_max_size () == get_size ())
        throw_allocation_size_error ();

      const size_ty offset = internal_range_length (begin_ptr (), pos);

      const size_ty new_size     = get_size () + 1;
      const size_ty new_capacity = calculate_new_capacity (new_size);

      ptr new_data_ptr  = unchecked_allocate (new_capacity, uninitialized_end_ptr ());
      ptr new_first     = unchecked_next (new_data_ptr, offset);
      ptr new_last      = new_first;

      try
      {
        construct (new_first, std::forward<Args> (args)...);
        unchecked_advance (new_last, 1);

        if (offset == get_size ()) // appending; strong exception guarantee
          uninitialized_move_if_noexcept (begin_ptr (), end_ptr (), new_data_ptr);
        else
        {
          uninitialized_move (begin_ptr (), pos, new_data_ptr);
          new_first = new_data_ptr;
          uninitialized_move (pos, end_ptr (), new_last);
        }
      }
      catch (...)
      {
        destroy_range (new_first, new_last);
        deallocate (new_data_ptr, new_capacity);
        throw;
      }

      destroy_range (begin_ptr (), end_ptr ());
      if (has_allocation ())
        deallocate (begin_ptr (), get_capacity ());

      set_data (new_data_ptr, new_capacity, new_size);
      return unchecked_next (begin_ptr (), offset);
    }

    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    auto
    small_vector_base<Allocator, InlineCapacity>::
    insert_copies (ptr pos, size_ty count, const value_t& val)
      ->ptr
    {
      if (count == 0)
        return pos;

      if (pos == end_ptr ())
        return append_copies (count, val);

      if (count <= num_uninitialized ())
      {
        // if we have fewer to insert than tailing elements after
        // `pos` we shift into uninitialized and then copy over
        const size_ty tail_size = internal_range_length (pos, end_ptr ());
        if (count < tail_size)
        {
          ptr inserted_end = shift_into_uninitialized (pos, count);

          // attempt to copy over the elements
          // if we fail we'll attempt a full roll-back
          try
          {
            std::fill (pos, inserted_end, val);
          }
          catch (...)
          {
            ptr original_end = move_left (inserted_end, end_ptr (), pos);
            destroy_range (original_end, end_ptr ());
            decrease_size (count);
            throw;
          }
        }
        else
        {
          // The number inserted is larger than the number after `pos`,
          // so part of the input will need to be assigned to existing
          // elements, and part of it will construct new elements.
          // In order:
          //   construct new elements from the input
          //   move construct existing elements over to the tail
          //   assign existing elements using the input

          ptr original_end = end_ptr ();

          // place a portion of the input into the uninitialized section
          size_ty num_val_constructed = count - tail_size;

          uninitialized_fill (end_ptr (), unchecked_next (end_ptr (), num_val_constructed), val);
          increase_size (num_val_constructed);

          try
          {
            // now move the tail to the end
            uninitialized_move (pos, original_end, end_ptr ());
            increase_size (tail_size);

            try
            {
              // finally, try to copy the rest of the elements over
              std::fill (pos, unchecked_next (pos, tail_size), val);
            }
            catch (...)
            {
              // attempt to roll back and destroy the tail if we fail
              ptr inserted_end = unchecked_prev (end_ptr (), tail_size);
              move_left (inserted_end, end_ptr (), pos);
              destroy_range (inserted_end, end_ptr ());
              decrease_size (tail_size);
              throw;
            }
          }
          catch (...)
          {
            // destroy the elements constructed from the input
            destroy_range (original_end, end_ptr ());
            decrease_size (num_val_constructed);
            throw;
          }
        }
        return pos;
      }
      else
      {
        // reallocate
        if (get_max_size () - get_size () < count)
          throw_allocation_size_error ();

        const size_ty offset = internal_range_length (begin_ptr (), pos);

        const size_ty new_size     = get_size () + count;
        const size_ty new_capacity = calculate_new_capacity (new_size);

        ptr new_data_ptr  = unchecked_allocate (new_capacity, uninitialized_end_ptr ());
        ptr new_first     = unchecked_next (new_data_ptr, offset);
        ptr new_last      = new_first;

        try
        {
          uninitialized_fill (new_first, unchecked_next (new_first, count), val);
          unchecked_advance  (new_last, count);

          if (count == 1 && pos == end_ptr ())
            uninitialized_move_if_noexcept (begin_ptr (), end_ptr (), new_data_ptr);
          else
          {
            uninitialized_move (begin_ptr (), pos, new_data_ptr);
            new_first = new_data_ptr;
            uninitialized_move (pos, end_ptr (), new_last);
          }
        }
        catch (...)
        {
          destroy_range (new_first, new_last);
          deallocate (new_data_ptr, new_capacity);
          throw;
        }

        destroy_range (begin_ptr (), end_ptr ());
        if (has_allocation ())
          deallocate (begin_ptr (), get_capacity ());

        set_data (new_data_ptr, new_capacity, new_size);
        return unchecked_next (begin_ptr (), offset);
      }
    }

    template <typename Allocator, unsigned InlineCapacity>
#ifdef GCH_LIB_CONCEPTS
    template <std::forward_iterator ForwardIt>
#else
    template <typename ForwardIt>
#endif
    GCH_CPP20_CONSTEXPR
    auto
    small_vector_base<Allocator, InlineCapacity>::
    insert_range (ptr pos, ForwardIt first, ForwardIt last, std::forward_iterator_tag)
      -> ptr
    {
      using iterator_cat = typename std::iterator_traits<ForwardIt>::iterator_category;
      if (first == last)
        return pos;

      if (pos == end_ptr ())
        return append_range (first, last, iterator_cat { });

      const size_ty num_insert = external_range_length (first, last);
      if (num_insert <= num_uninitialized ())
      {
        // if we have fewer to insert than tailing elements after
        // `pos` we shift into uninitialized and then copy over
        const size_ty tail_size = internal_range_length (pos, end_ptr ());
        if (num_insert < tail_size)
        {
          shift_into_uninitialized (pos, num_insert);

          // attempt to copy over the elements
          // if we fail we'll attempt a full roll-back
          try
          {
            std::copy (first, last, pos);
          }
          catch (...)
          {
            ptr inserted_end = unchecked_next (pos, num_insert);
            ptr original_end = move_left (inserted_end, end_ptr (), pos);
            destroy_range (original_end, end_ptr ());
            decrease_size (num_insert);
            throw;
          }
        }
        else
        {
          // using the same method as insert_copies

          ptr original_end = end_ptr ();
          ForwardIt pivot  = unchecked_next (first, tail_size);

          // place a portion of the input into the uninitialized section
          uninitialized_copy (pivot, last, end_ptr ());
          increase_size (num_insert - tail_size);

          try
          {
            // now move the tail to the end
            uninitialized_move (pos, original_end, end_ptr ());
            increase_size (tail_size);

            try
            {
              // finally, try to copy the rest of the elements over
              std::copy (first, pivot, pos);
            }
            catch (...)
            {
              // attempt to roll back and destroy the tail if we fail
              ptr inserted_end = unchecked_prev (end_ptr (), tail_size);
              move_left (inserted_end, end_ptr (), pos);
              destroy_range (inserted_end, end_ptr ());
              decrease_size (tail_size);
              throw;
            }
          }
          catch (...)
          {
            // if we throw destroy the first copy we made
            destroy_range (original_end, end_ptr ());
            decrease_size (num_insert - tail_size);
            throw;
          }
        }
        return pos;
      }
      else
      {
        // reallocate
        if (get_max_size () - get_size () < num_insert)
          throw_allocation_size_error ();

        const size_ty offset = internal_range_length (begin_ptr (), pos);

        const size_ty new_size     = get_size () + num_insert;
        const size_ty new_capacity = calculate_new_capacity (new_size);

        ptr new_data_ptr  = unchecked_allocate (new_capacity, uninitialized_end_ptr ());
        ptr new_first     = unchecked_next (new_data_ptr, offset);
        ptr new_last      = new_first;

        try
        {
          uninitialized_copy (first, last, new_first);
          unchecked_advance  (new_last, num_insert);

          if (num_insert == 1 && pos == end_ptr ())
            uninitialized_move_if_noexcept (begin_ptr (), end_ptr (), new_data_ptr);
          else
          {
            uninitialized_move (begin_ptr (), pos, new_data_ptr);
            new_first = new_data_ptr;
            uninitialized_move (pos, end_ptr (), new_last);
          }
        }
        catch (...)
        {
          destroy_range (new_first, new_last);
          deallocate (new_data_ptr, new_capacity);
          throw;
        }

        destroy_range (begin_ptr (), end_ptr ());
        if (has_allocation ())
          deallocate (begin_ptr (), get_capacity ());

        set_data (new_data_ptr, new_capacity, new_size);
        return unchecked_next (begin_ptr (), offset);
      }
    }

    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    auto
    small_vector_base<Allocator, InlineCapacity>::
    shrink_to_size (void)
      -> ptr
    {
      if (! has_allocation () || get_size () == get_capacity ())
        return begin_ptr ();

      if (get_size () <= InlineCapacity)
      {
        // we move to inline storage
        size_ty new_capacity;
        ptr     new_data_ptr;

#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
        if (std::is_constant_evaluated ())
        {
          new_capacity = constexpr_inline_capacity;
          new_data_ptr = unchecked_allocate (new_capacity);
        }
        else
        {
          new_capacity = InlineCapacity;
          new_data_ptr = storage_ptr ();
        }
#else
        new_capacity = InlineCapacity;
        new_data_ptr = storage_ptr ();
#endif

        uninitialized_move (begin_ptr (), end_ptr (), new_data_ptr);

        destroy_range (begin_ptr (), end_ptr ());
        deallocate (data_ptr (), get_capacity ());

        set_data_ptr (new_data_ptr);
        set_capacity (new_capacity);
      }
      else
      {
        const size_ty new_capacity = get_size ();
        const ptr     new_data_ptr = unchecked_allocate (new_capacity, uninitialized_end_ptr ());

        uninitialized_move (begin_ptr (), end_ptr (), new_data_ptr);

        destroy_range (begin_ptr (), end_ptr ());
        deallocate (begin_ptr (), get_capacity ());

        set_data_ptr (new_data_ptr);
        set_capacity (new_capacity);
      }
      return begin_ptr ();
    }

    template <typename Allocator, unsigned InlineCapacity>
    template <typename V>
    GCH_CPP20_CONSTEXPR
    typename std::enable_if<std::is_move_constructible<V>::value
                        &&  std::is_move_assignable<V>::value
#ifdef GCH_LIB_IS_SWAPPABLE
                        &&  std::is_swappable<V>::value
#endif
                            >::type
    small_vector_base<Allocator, InlineCapacity>::
    swap (small_vector_base& other)
      noexcept ((std::allocator_traits<alloc_t>::propagate_on_container_swap::value
#ifdef GCH_LIB_IS_ALWAYS_EQUAL
             ||  std::allocator_traits<alloc_t>::is_always_equal::value
#endif
                 )
#ifdef GCH_LIB_IS_SWAPPABLE
            &&  std::is_nothrow_swappable<value_t>::value
#else
            &&  detail::small_vector_adl::is_nothrow_swappable<value_t>::value
#endif
            &&  std::is_nothrow_move_constructible<value_t>::value)
    {
      // note: no-op for std::allocator
      alloc_interface::swap (other);

      using std::swap;

      if (! has_allocation ())
      {
        if (! other.has_allocation ())
        {
          std::swap_ranges (begin_ptr (), end_ptr (), other.begin_ptr ());
          // data_ptr still equal to storage_ptr ()
          // capacity still equal to InlineCapacity
        }
        else
        {
          uninitialized_move (begin_ptr (), end_ptr (), other.storage_ptr ());
          set_data_ptr (other.data_ptr ());
          set_capacity (other.get_capacity ());
        }

        other.set_data_ptr (other.storage_ptr ());
        other.set_capacity (InlineCapacity);
        swap_size (other);
      }
      else if (! other.has_allocation ())
      {
        // other is inline
        // *this is not
        uninitialized_move (other.begin_ptr (), other.end_ptr (), storage_ptr ());
        other.set_data_ptr (data_ptr ());
        other.set_capacity (get_capacity ());

        set_data_ptr (storage_ptr ());
        set_capacity (InlineCapacity);
        swap_size (other);
      }
      else // neither are inline so just swap
        swap_data (other);
    }

  } // detail

  template <typename T, unsigned InlineCapacity, typename Allocator>
#ifdef GCH_LIB_CONCEPTS
  requires concepts::Allocator<Allocator, T>
#endif
  class small_vector
    : private detail::small_vector_base<Allocator, InlineCapacity>
  {
    using base = detail::small_vector_base<Allocator, InlineCapacity>;

  public:
    static_assert (std::is_same<T, typename Allocator::value_type>::value,
                   "`Allocator::value_type` must be the same as `T`.");

    using value_type             = T;
    using allocator_type         = Allocator;
    using size_type              = typename base::size_type;
    using difference_type        = typename base::difference_type;
    using reference              =       value_type&;
    using const_reference        = const value_type&;
    using pointer                = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer          = typename std::allocator_traits<allocator_type>::const_pointer;

    using iterator               = small_vector_iterator<pointer, difference_type>;
    using const_iterator         = small_vector_iterator<const_pointer, difference_type>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

#ifdef GCH_LIB_CONCEPTS

  private:
    static constexpr
    auto
    Destructible = concepts::Destructible<value_type>;

    static constexpr
    auto
    MoveAssignable = concepts::MoveAssignable<value_type>;

    static constexpr
    auto
    CopyAssignable = concepts::CopyAssignable<value_type>;

    static constexpr
    auto
    MoveConstructible = concepts::MoveConstructible<value_type>;

    static constexpr
    auto
    CopyConstructible = concepts::CopyConstructible<value_type>;

    static constexpr
    auto
    Swappable = concepts::Swappable<value_type>;

    static constexpr
    auto
    DefaultInsertable = concepts::DefaultInsertable<value_type, small_vector, allocator_type>;

    static constexpr
    auto
    MoveInsertable = concepts::MoveInsertable<value_type, small_vector, allocator_type>;

    static constexpr
    auto
    CopyInsertable = concepts::CopyInsertable<value_type, small_vector, allocator_type>;

    static constexpr
    auto
    Erasable = concepts::Erasable<value_type, small_vector, allocator_type>;

    template <typename ...Args>
    struct EmplaceConstructible
      : std::integral_constant<bool,
          concepts::EmplaceConstructible<value_type, small_vector, allocator_type, Args...>>
    { };

    template <typename U>
    struct AssignableFrom
      : std::integral_constant<bool, std::assignable_from<value_type&, U>>
    { };

  public:

#endif

    /* constructors */
    GCH_CPP20_CONSTEXPR
    small_vector (void)
      noexcept (noexcept (allocator_type ()))
#ifdef GCH_LIB_CONCEPTS
      requires concepts::DefaultConstructible<allocator_type>
#endif
    = default;


    GCH_CPP20_CONSTEXPR
    small_vector (const small_vector& other)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable && CopyAssignable
#endif
    = default;

    GCH_CPP20_CONSTEXPR
    small_vector (small_vector&& other) noexcept
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable
#endif
    = default;

    GCH_CPP20_CONSTEXPR explicit
    small_vector (const allocator_type& alloc) noexcept
      : base (alloc)
    { }

    GCH_CPP20_CONSTEXPR
    small_vector (const small_vector& other, const allocator_type& alloc)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable
#endif
      : base (other, alloc)
    { }

    GCH_CPP20_CONSTEXPR
    small_vector (small_vector&& other, const allocator_type& alloc)
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable
#endif
      : base (std::move (other), alloc)
    { }

    GCH_CPP20_CONSTEXPR explicit
    small_vector (size_type count, const allocator_type& alloc = allocator_type ())
#ifdef GCH_LIB_CONCEPTS
      requires DefaultInsertable
#endif
      : base (count, alloc)
    { }

    GCH_CPP20_CONSTEXPR explicit
    small_vector (size_type count, const_reference value,
                  const allocator_type& alloc = allocator_type ())
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable
#endif
      : base (count, value, alloc)
    { }

#ifdef GCH_LIB_CONCEPTS
    template <std::input_iterator InputIt>
#else
    template <typename InputIt,
              typename = typename std::enable_if<std::is_base_of<std::input_iterator_tag,
                           typename std::iterator_traits<InputIt>::iterator_category>::value>::type>
#endif
    GCH_CPP20_CONSTEXPR
    small_vector (InputIt first, InputIt last, const allocator_type& alloc = allocator_type ())
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<decltype (*first)>::value
          &&  (std::forward_iterator<InputIt> || MoveInsertable)
#endif
      : base (first, last, typename std::iterator_traits<InputIt>::iterator_category { }, alloc)
    { }

    GCH_CPP20_CONSTEXPR
    small_vector (std::initializer_list<value_type> init,
                  const allocator_type& alloc = allocator_type ())
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<decltype (*std::begin (init))>::value
#endif
      : small_vector (init.begin (), init.end (), alloc)
    { }

    /* destructor */
    GCH_CPP20_CONSTEXPR
    ~small_vector (void) = default;

    /* assignment */
    GCH_CPP20_CONSTEXPR
    small_vector&
    operator= (const small_vector& other)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable && CopyAssignable
#endif
    {
      if (&other != this)
        base::operator= (other);
      return *this;
    }

    GCH_CPP20_CONSTEXPR
    small_vector&
    operator= (small_vector&& other) noexcept
#ifdef GCH_LIB_CONCEPTS
      // note: the standard calls says
      // std::allocator_traits<allocator_type>::propagate_on_container_move_assignment == false
      // implies MoveInsertable && MoveAssignable here, but since we have
      // inline storage we must always require moves [tab:container.alloc.req]
      requires MoveInsertable && MoveAssignable
#endif
    {
      if (&other != this)
        base::operator= (std::move (other));
      return *this;
    }

    GCH_CPP20_CONSTEXPR
    small_vector&
    operator= (std::initializer_list<value_type> ilist)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable && CopyAssignable
#endif
    {
      assign (ilist);
      return *this;
    }

    GCH_CPP20_CONSTEXPR
    void
    assign (size_type count, const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable && CopyAssignable
#endif
    {
      base::assign_copies (count, value);
    }

    // TODO: Don't require AssignableFrom, sfinae different versions for each case in the impl.
#ifdef GCH_LIB_CONCEPTS
    template <std::input_iterator InputIt>
#else
    template <typename InputIt,
              typename = typename std::enable_if<std::is_base_of<std::input_iterator_tag,
                           typename std::iterator_traits<InputIt>::iterator_category>::value>::type>
#endif
    GCH_CPP20_CONSTEXPR
    void
    assign (InputIt first, InputIt last)
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<decltype (*first)>::value
           &&  AssignableFrom<decltype (*first)>::value
           &&  (std::forward_iterator<InputIt> || MoveInsertable)
#endif
    {
      using iterator_cat = typename std::iterator_traits<InputIt>::iterator_category;
      base::assign_range (first, last, iterator_cat { });
    }

    GCH_CPP20_CONSTEXPR
    void
    assign (std::initializer_list<value_type> ilist)
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<decltype (*std::begin (ilist))>::value
           &&  AssignableFrom<decltype (*std::begin (ilist))>::value
#endif
    {
      assign (ilist.begin (), ilist.end ());
    }

    /* swap */
    // note: dispariate with the standard
#ifndef GCH_LIB_CONCEPTS
    template <typename ValueType = value_type,
              typename std::enable_if<std::is_move_constructible<ValueType>::value
                                  &&  std::is_move_assignable<ValueType>::value
#ifdef GCH_LIB_IS_SWAPPABLE
                                  &&  std::is_swappable<ValueType>::value
#endif
                                      >::type * = nullptr>
#endif
    GCH_CPP20_CONSTEXPR
    void
    swap (small_vector& other)
      noexcept ((std::allocator_traits<allocator_type>::propagate_on_container_swap::value
#ifdef GCH_LIB_IS_ALWAYS_EQUAL
             ||  std::allocator_traits<allocator_type>::is_always_equal::value
#endif
                 )
#ifdef GCH_LIB_IS_SWAPPABLE
            &&  std::is_nothrow_swappable<value_type>::value
#else
            &&  detail::small_vector_adl::is_nothrow_swappable<value_type>::value
#endif
            &&  std::is_nothrow_move_constructible<value_type>::value)
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable && Swappable
#endif
    {
      // base handles the storage swap if needed
      base::swap (other);
    }

    /* iteration */
    GCH_CPP14_CONSTEXPR
    iterator
    begin (void) noexcept
    {
      return iterator { base::begin_ptr () };
    }

    constexpr
    const_iterator
    begin (void) const noexcept
    {
      return const_iterator { base::begin_ptr () };
    }

    constexpr
    const_iterator
    cbegin (void) const noexcept
    {
      return begin ();
    }

    GCH_CPP14_CONSTEXPR
    iterator
    end (void) noexcept
    {
      return iterator { base::end_ptr () };
    }

    constexpr
    const_iterator
    end (void) const noexcept
    {
      return const_iterator { base::end_ptr () };
    }

    constexpr
    const_iterator
    cend (void) const noexcept
    {
      return end ();
    }

    GCH_CPP14_CONSTEXPR
    reverse_iterator
    rbegin (void) noexcept
    {
      return reverse_iterator { end () };
    }

    constexpr
    const_reverse_iterator
    rbegin (void) const noexcept
    {
      return const_reverse_iterator { end () };
    }

    constexpr
    const_reverse_iterator
    crbegin (void) const noexcept
    {
      return rbegin ();
    }

    GCH_CPP14_CONSTEXPR
    reverse_iterator
    rend (void) noexcept
    {
      return reverse_iterator { begin () };
    }

    constexpr
    const_reverse_iterator
    rend (void) const noexcept
    {
      return const_reverse_iterator { begin () };
    }

    constexpr
    const_reverse_iterator
    crend (void) const noexcept
    {
      return rend ();
    }

    /* access */
    GCH_CPP14_CONSTEXPR
    reference
    at (size_type pos)
    {
      if (size () <= pos)
        base::throw_index_error ();
      return begin ()[static_cast<difference_type> (pos)];
    }

    GCH_CPP14_CONSTEXPR
    const_reference
    at (size_type pos) const
    {
      if (size () <= pos)
        base::throw_index_error ();
      return begin ()[static_cast<difference_type> (pos)];
    }

    GCH_CPP14_CONSTEXPR
    reference
    operator[] (size_type pos) noexcept
    {
      return begin ()[static_cast<difference_type> (pos)];
    }

    constexpr
    const_reference
    operator[] (size_type pos) const noexcept
    {
      return begin ()[static_cast<difference_type> (pos)];
    }

    GCH_CPP14_CONSTEXPR
    reference
    front (void) noexcept
    {
      return (*this)[0];
    }

    constexpr
    const_reference
    front (void) const noexcept
    {
      return (*this)[0];
    }

    GCH_CPP14_CONSTEXPR
    reference
    back (void) noexcept
    {
      return (*this)[size () - 1];
    }

    constexpr
    const_reference
    back (void) const noexcept
    {
      return (*this)[size () - 1];
    }

    GCH_CPP14_CONSTEXPR
    pointer
    data (void) noexcept
    {
      return base::begin_ptr ();
    }

    constexpr
    const_pointer
    data (void) const noexcept
    {
      return base::begin_ptr ();
    }

    /* state information */
    constexpr
    size_type
    size (void) const noexcept
    {
      return static_cast<size_type> (base::get_size ());
    }

    GCH_NODISCARD constexpr
    bool
    empty (void) const noexcept
    {
      return size () == 0;
    }

    GCH_CPP14_CONSTEXPR
    size_type
    max_size (void) const noexcept
    {
      return static_cast<size_type> (base::get_max_size ());
    }

    constexpr
    size_type
    capacity (void) const noexcept
    {
      return static_cast<size_type> (base::get_capacity ());
    }

    constexpr
    allocator_type
    get_allocator (void) const noexcept
    {
      return base::copy_allocator ();
    }

    /* insertion */
    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable && CopyAssignable
#endif
    {
      return emplace (pos, value);
    }

    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, value_type&& value)
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable && MoveAssignable
#endif
    {
      return emplace (pos, std::move (value));
    }

    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, size_type count, const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable && CopyAssignable
#endif
    {
      return iterator (base::insert_copies (base::ptr_cast (pos), count, value));
    }

#ifdef GCH_LIB_CONCEPTS
    template <std::input_iterator InputIt>
#else
    template <typename InputIt,
              typename = typename std::enable_if<std::is_base_of<std::input_iterator_tag,
                           typename std::iterator_traits<InputIt>::iterator_category>::value>::type>
#endif
    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, InputIt first, InputIt last)
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<decltype (*first)>::value
           &&  MoveInsertable
           &&  MoveConstructible
           &&  MoveAssignable
           &&  Swappable
#endif
    {
      using iterator_cat = typename std::iterator_traits<InputIt>::iterator_category;
      return iterator (base::insert_range (base::ptr_cast (pos), first, last, iterator_cat { }));
    }


    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, std::initializer_list<value_type> ilist)
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<decltype (*std::begin (ilist))>::value
           &&  MoveInsertable
           &&  MoveConstructible
           &&  MoveAssignable
           &&  Swappable
#endif
    {
      return insert (pos, ilist.begin (), ilist.end ());
    }

    template <typename ...Args>
    GCH_CPP20_CONSTEXPR
    iterator
    emplace (const_iterator pos, Args&&... args)
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<Args...>::value
           &&  MoveInsertable
           &&  MoveAssignable
#endif
    {
      return iterator (base::emplace_at (base::ptr_cast (pos), std::forward<Args> (args)...));
    }

    GCH_CPP20_CONSTEXPR
    iterator
    erase (const_iterator pos)
#ifdef GCH_LIB_CONCEPTS
      requires MoveAssignable
#endif
    {
      GCH_ASSERT (begin () <= pos
              &&  "The argument `pos` is out of bounds (before `begin ()`)."   );

      GCH_ASSERT (pos < end ()
              &&  "The argument `pos` is out of bounds (at or after `end ()`).");
      return iterator (base::erase_at (base::ptr_cast (pos)));
    }

    GCH_CPP20_CONSTEXPR
    iterator
    erase (const_iterator first, const_iterator last)
#ifdef GCH_LIB_CONCEPTS
      requires MoveAssignable
#endif
    {
      GCH_ASSERT (first <= last && "Invalid range.");

      GCH_ASSERT (begin () <= first
              &&  "The argument `first` is out of bounds (before `begin ()`)."  );

      GCH_ASSERT (last <= end ()
              &&  "The argument `last` is out of bounds (after `end ()`).");

      return iterator (base::erase_range (base::ptr_cast (first), base::ptr_cast(last)));
    }

    GCH_CPP20_CONSTEXPR
    void
    push_back (const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable
#endif
    {
      emplace_back (value);
    }

    GCH_CPP20_CONSTEXPR
    void
    push_back (value_type&& value)
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable
#endif
    {
      emplace_back (std::move (value));
    }

    template <typename ...Args>
    GCH_CPP20_CONSTEXPR
    reference
    emplace_back (Args&&... args)
#ifdef GCH_LIB_CONCEPTS
      requires EmplaceConstructible<Args...>::value && MoveInsertable
#endif
    {
      return *base::append_element (std::forward<Args> (args)...);
    }

    GCH_CPP20_CONSTEXPR
    void
    pop_back (void)
#ifdef GCH_LIB_CONCEPTS
      requires Erasable
#endif
    {
      GCH_ASSERT (! empty () && "`pop_back ()` called on an empty `small_vector`.");
      base::erase_last ();
    }

    /* global state modification */
    GCH_CPP20_CONSTEXPR
    void
    reserve (size_type new_cap)
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable
#endif
    {
      base::request_capacity (new_cap);
    }

    GCH_CPP20_CONSTEXPR
    void
    shrink_to_fit (void)
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable
#endif
    {
      base::shrink_to_size ();
    }

    GCH_CPP20_CONSTEXPR
    void
    clear (void) noexcept
#ifdef GCH_LIB_CONCEPTS
      requires Erasable
#endif
    {
      base::erase_all ();
    }

    GCH_CPP20_CONSTEXPR
    void
    resize (size_type count)
#ifdef GCH_LIB_CONCEPTS
      requires MoveInsertable && DefaultInsertable
#endif
    {
      base::resize_default (count);
    }

    GCH_CPP20_CONSTEXPR
    void
    resize (size_type count, const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires CopyInsertable
#endif
    {
      if (count == 0)
        clear ();

      if (size () < count)
        insert (cend (), count - size (), value);
      else if (count < size ())
        erase (base::unchecked_next (cbegin (), count), cend ());
    }

    /* non-standard */

    GCH_NODISCARD constexpr
    bool
    inlined (void) const noexcept
    {
      return ! base::has_allocation ();
    }

    GCH_NODISCARD constexpr
    bool
    inlinable (void) const noexcept
    {
      return base::is_inlinable ();
    }

    GCH_NODISCARD constexpr
    size_type
    inline_capacity (void) const noexcept
    {
      return InlineCapacity;
    }
  };

  /* non-member functions */

  template <typename T, unsigned InlineCapacity, typename Allocator>
  GCH_CPP20_CONSTEXPR
  bool
  operator== (const small_vector<T, InlineCapacity, Allocator>& lhs,
              const small_vector<T, InlineCapacity, Allocator>& rhs)
  {
    return lhs.size () == rhs.size () && std::equal (lhs.begin (), lhs.end (),
                                                     rhs.begin (), rhs.end ());
  }

#ifdef GCH_LIB_THREE_WAY_COMPARISON

  template <typename T, unsigned InlineCapacity, typename Allocator>
  constexpr
  auto
  operator<=> (const small_vector<T, InlineCapacity, Allocator>& lhs,
               const small_vector<T, InlineCapacity, Allocator>& rhs)
    requires std::three_way_comparable_with<T, T>
  {
    return std::lexicographical_compare_three_way (lhs.begin (), lhs.end (),
                                                   rhs.begin (), rhs.end (),
                                                   std::compare_three_way { });
  }

  template <typename T, unsigned InlineCapacity, typename Allocator>
  constexpr
  auto
  operator<=> (const small_vector<T, InlineCapacity, Allocator>& lhs,
               const small_vector<T, InlineCapacity, Allocator>& rhs)
    requires (! std::three_way_comparable_with<T, T>)
  {
    constexpr auto comparison = [](const T& l, const T& r)
                                {
                                  return (l < r) ? std::weak_ordering::less
                                                 : (r < l) ? std::weak_ordering::greater
                                                           : std::weak_ordering::equivalent;
                                };
    return std::lexicographical_compare_three_way (lhs.begin (), lhs.end (),
                                                   rhs.begin (), rhs.end (), comparison);
  }

#else

  template <typename T, unsigned InlineCapacity, typename Allocator>
  bool
  operator!= (const small_vector<T, InlineCapacity, Allocator>& lhs,
              const small_vector<T, InlineCapacity, Allocator>& rhs)
  {
    return ! (lhs == rhs);
  }

  template <typename T, unsigned InlineCapacity, typename Allocator>
  bool
  operator<  (const small_vector<T, InlineCapacity, Allocator>& lhs,
              const small_vector<T, InlineCapacity, Allocator>& rhs)
  {
    return std::lexicographical_compare (lhs.begin (), lhs.end (), rhs.begin (), rhs.end ());
  }

  template <typename T, unsigned InlineCapacity, typename Allocator>
  bool
  operator<= (const small_vector<T, InlineCapacity, Allocator>& lhs,
              const small_vector<T, InlineCapacity, Allocator>& rhs)
  {
    return ! (lhs > rhs);
  }

  template <typename T, unsigned InlineCapacity, typename Allocator>
  bool
  operator>  (const small_vector<T, InlineCapacity, Allocator>& lhs,
              const small_vector<T, InlineCapacity, Allocator>& rhs)
  {
    return rhs < lhs;
  }

  template <typename T, unsigned InlineCapacity, typename Allocator>
  bool
  operator>= (const small_vector<T, InlineCapacity, Allocator>& lhs,
              const small_vector<T, InlineCapacity, Allocator>& rhs)
  {
    return ! (lhs < rhs);
  }

#endif

  template <typename T, unsigned InlineCapacity, typename Allocator,
            typename std::enable_if<std::is_move_constructible<T>::value
                                &&  std::is_move_assignable<T>::value
#ifdef GCH_LIB_IS_SWAPPABLE
                                &&  std::is_swappable<T>::value
#endif
                                    >::type * = nullptr>
  GCH_CPP20_CONSTEXPR
  void
  swap (small_vector<T, InlineCapacity, Allocator>& lhs,
        small_vector<T, InlineCapacity, Allocator>& rhs)
    noexcept (noexcept (lhs.swap (rhs)))
  {
    lhs.swap (rhs);
  }

  template <typename T, unsigned InlineCapacity, typename Allocator, typename U>
  GCH_CPP20_CONSTEXPR
  typename small_vector<T, InlineCapacity, Allocator>::size_type
  erase (small_vector<T, InlineCapacity, Allocator>& c, const U& value)
  {
    const auto initial_size = c.size ();
    c.erase (std::remove (c.begin (), c.end (), value), c.end ());
    return initial_size - c.size ();
  }

  template <typename T, unsigned InlineCapacity, typename Allocator, typename Pred>
  GCH_CPP20_CONSTEXPR
  typename small_vector<T, InlineCapacity, Allocator>::size_type
  erase_if (small_vector<T, InlineCapacity, Allocator>& c, Pred pred)
  {
    const auto initial_size = c.size ();
    c.erase (std::remove_if (c.begin (), c.end (), pred), c.end ());
    return initial_size - c.size ();
  }

#ifdef GCH_CTAD_SUPPORT

  template <typename InputIt,
            unsigned InlineCapacity = default_buffer_size<
              std::allocator<typename std::iterator_traits<InputIt>::value_type>>::value,
            typename Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>>
  small_vector (InputIt, InputIt, Allocator = Allocator ())
    -> small_vector<typename std::iterator_traits<InputIt>::value_type, InlineCapacity, Allocator>;

#endif

} // namespace gch

#endif // GCH_SMALL_VECTOR_HPP
