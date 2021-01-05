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

#ifndef GCH_CPP14_CONSTEXPR
#  if __cpp_constexpr >= 201304L
#    define GCH_CPP14_CONSTEXPR constexpr
#  else
#    define GCH_CPP14_CONSTEXPR
#  endif
#endif

#ifndef GCH_CPP17_CONSTEXPR
#  if __cpp_constexpr >= 201603L
#    define GCH_CPP17_CONSTEXPR constexpr
#  else
#    define GCH_CPP17_CONSTEXPR
#  endif
#endif

#ifndef GCH_CPP20_CONSTEXPR
#  if __cpp_constexpr >= 201907L
#    define GCH_CPP20_CONSTEXPR constexpr
#  else
#    define GCH_CPP20_CONSTEXPR
#  endif
#endif

#ifndef GCH_INLINE_VAR
#  if __cpp_inline_variables >= 201606
#    define GCH_INLINE_VAR inline
#  else
#    define GCH_INLINE_VAR
#  endif
#endif

#ifndef GCH_NORETURN
#  ifdef __has_cpp_attribute
#    if __has_cpp_attribute(noreturn) >= 200809L
#      define GCH_NORETURN [[noreturn]]
#    else
#      define GCH_NORETURN
#    endif
#  else
#    define GCH_NORETURN
#  endif
#endif

#ifndef GCH_NODISCARD
#  ifdef __has_cpp_attribute
#    if __has_cpp_attribute(nodiscard) >= 201603L
#      define GCH_NODISCARD [[nodiscard]]
#    else
#      define GCH_NODISCARD
#    endif
#  else
#    define GCH_NODISCARD
#  endif
#endif

#ifndef GCH_EMPTY_BASE
#  if _MSC_VER >= 1910
#    define GCH_EMPTY_BASE __declspec(empty_bases)
#  else
#    define GCH_EMPTY_BASE
#  endif
#endif

#if __cpp_impl_three_way_comparison >= 201907L
#  ifndef GCH_IMPL_THREE_WAY_COMPARISON
#    define GCH_IMPL_THREE_WAY_COMPARISON
#  endif
#  if __has_include(<compare>)
#    include <compare>
#    if __cpp_lib_three_way_comparison >= 201907L
#      ifndef GCH_LIB_THREE_WAY_COMPARISON
#        define GCH_LIB_THREE_WAY_COMPARISON
#      endif
#    endif
#  endif
#endif

#if __cpp_lib_erase_if >= 202002L
#  ifndef GCH_LIB_ERASE_IF
#    define GCH_LIB_ERASE_IF
#  endif
#endif

#if __cpp_deduction_guides >= 201703
#  ifndef GCH_CTAD_SUPPORT
#    define GCH_CTAD_SUPPORT
#  endif
#endif

#if __cpp_exceptions >= 199711L
#  ifndef GCH_EXCEPTIONS
#    define GCH_EXCEPTIONS
#  endif
#endif

#if __cpp_lib_is_final >= 201402L
#  ifndef GCH_LIB_IS_FINAL
#    define GCH_LIB_IS_FINAL
#  endif
#endif

#if __cpp_lib_is_constant_evaluated >= 201811L
#  ifndef GCH_LIB_IS_CONSTANT_EVALUATED
#    define GCH_LIB_IS_CONSTANT_EVALUATED
#  endif
#endif

#if __cplusplus >= 202002L
#  ifndef GCH_CONTIGUOUS_ITERATOR
#    define GCH_CONTIGUOUS_ITERATOR
#  endif
#endif

#if __cpp_lib_allocator_traits_is_always_equal >= 201411L
#  ifndef GCH_LIB_IS_ALWAYS_EQUAL
#    define GCH_LIB_IS_ALWAYS_EQUAL
#  endif
#endif

#if __cpp_concepts >= 201907L
#  ifndef GCH_CONCEPTS
#    define GCH_CONCEPTS
#  endif
#  if defined(__has_include) && __has_include(<concepts>)
#    include <concepts>
#    if __cpp_lib_concepts >= 202002L
#      ifndef GCH_LIB_CONCEPTS
#        define GCH_LIB_CONCEPTS
#      endif
#    endif
#  endif
#endif

#include <algorithm>
#include <cassert>
#include <cstring>
#include <initializer_list>
#include <iterator>
#include <memory>

#ifndef GCH_CONTIGUOUS_ITERATOR
#  include <vector>
#endif

#ifdef GCH_EXCEPTIONS
#  include <stdexcept>
#endif

namespace gch
{

  #ifdef GCH_LIB_CONCEPTS

  namespace concepts
  {

    template <typename T>
    concept NoThrowDestructible = std::is_nothrow_destructible<T>::value;

    template <typename T, typename... Args>
    concept NoThrowConstructibleFrom =
      NoThrowDestructible<T> && std::is_nothrow_constructible<T, Args...>::value;

    template <typename From, typename To>
    concept NoThrowConvertibleTo =
      std::is_nothrow_convertible<From, To>::value &&
      requires (typename std::add_rvalue_reference<From>::type (&f) ())
      {
        static_cast<To> (f ());
      };

    template <typename T>
    concept NoThrowMoveConstructible =
      NoThrowConstructibleFrom<T, T> &&
      NoThrowConvertibleTo<T, T>;

    template <typename T>
    concept NoThrowCopyConstructible =
      NoThrowMoveConstructible<T> &&
      NoThrowConstructibleFrom<T,       T&> && NoThrowConvertibleTo<      T&, T> &&
      NoThrowConstructibleFrom<T, const T&> && NoThrowConvertibleTo<const T&, T> &&
      NoThrowConstructibleFrom<T, const T > && NoThrowConvertibleTo<const T , T>;

    // note: std::default_initializable requires std::destructible
    template <typename T>
    concept DefaultConstructible =
      std::is_constructible_v<T> &&
      requires { T { }; } &&
      requires { ::new (static_cast<void *> (nullptr)) T; };

    template <typename T>
    concept MoveAssignable = std::assignable_from<T&, T&&>;

    template <typename T>
    concept CopyAssignable = MoveAssignable<T> &&
                             std::assignable_from<T&, T&> &&
                             std::assignable_from<T&, const T&>;

    template <typename T>
    concept MoveConstructible = std::move_constructible<T>;

    template <typename T>
    concept CopyConstructible = std::copy_constructible<T>;

    template <typename T>
    concept Destructible = std::is_destructible<T>::value;

    template <typename T>
    concept Swappable = std::swappable<T>;

    template <typename T>
    concept EqualityComparable = std::equality_comparable<T>;

    template <typename T, typename X, typename A>
    concept DefaultInsertable =
      std::same_as<typename X::value_type, T> &&
      std::same_as<typename X::allocator_type,
                   typename std::allocator_traits<A>::template rebind_alloc<T>> &&
      requires (A m, T *p)
      {
        std::allocator_traits<A>::construct (m, p);
      };

    template <typename T, typename X, typename A>
    concept MoveInsertable =
      std::same_as<typename X::value_type, T> &&
      std::same_as<typename X::allocator_type,
                   typename std::allocator_traits<A>::template rebind_alloc<T>> &&
      requires (A m, T *p, T rv)
      {
        std::allocator_traits<A>::construct (m, p, static_cast<T&&> (rv));
      };

    template <typename T, typename X, typename A>
    concept CopyInsertable =
      std::same_as<typename X::value_type, T> &&
      std::same_as<typename X::allocator_type,
                   typename std::allocator_traits<A>::template rebind_alloc<T>> &&
      MoveInsertable<T, X, A> &&
      requires (A m, T *p, T& v)
      {
        std::allocator_traits<A>::construct (m, p, v);
        std::allocator_traits<A>::construct (m, p, static_cast<const T&> (v));
      };

    template <typename T, typename X, typename A, typename ...Args>
    concept EmplaceConstructible =
      std::same_as<typename X::value_type, T> &&
      std::same_as<typename X::allocator_type,
                   typename std::allocator_traits<A>::template rebind_alloc<T>> &&
      requires (A m, T *p, Args&&... args)
      {
        std::allocator_traits<A>::construct (m, p, args...);
      };

    template <typename T, typename X, typename A>
    concept Erasable =
      std::same_as<typename X::value_type, T> &&
        std::same_as<typename X::allocator_type,
                     typename std::allocator_traits<A>::template rebind_alloc<T>> &&
        requires (A m, T *p) { std::allocator_traits<A>::destroy (m, p); };

    template <typename T>
    concept ContextuallyConvertibleToBool = std::constructible_from<bool, T>;

    template <typename T>
    concept BoolConstant =
      std::derived_from<T, std::true_type> ||
      std::derived_from<T, std::false_type>;

    template <typename T>
    concept NullablePointer =
      EqualityComparable<T> &&
      DefaultConstructible<T> &&
      CopyConstructible<T> &&
      CopyAssignable<T> &&
      Destructible<T> &&
      std::constructible_from<T, std::nullptr_t> &&
      std::convertible_to<std::nullptr_t, T> &&
      requires (T p, T q, std::nullptr_t np)
      {
        { T (np)  } -> std::same_as<T>;
        { p = np  } -> std::same_as<T&>;
        { p != q  } -> ContextuallyConvertibleToBool;
        { p == np } -> ContextuallyConvertibleToBool;
        { np == p } -> ContextuallyConvertibleToBool;
        { p != np } -> ContextuallyConvertibleToBool;
        { np != p } -> ContextuallyConvertibleToBool;
      };

    static_assert(  NullablePointer<int *>);
    static_assert(! NullablePointer<int>);

    template <typename A,
              typename U = typename std::allocator_traits<A>::value_type,
              typename B = typename std::allocator_traits<A>::template rebind_alloc<U>>
    concept Allocator =
      NoThrowCopyConstructible<A> &&
      requires (A a,
                B b,
                typename std::allocator_traits<A>::pointer p,
                typename std::allocator_traits<A>::const_pointer cp,
                typename std::allocator_traits<A>::void_pointer vp,
                typename std::allocator_traits<A>::const_void_pointer cvp,
                U *xp,
                typename std::allocator_traits<A>::value_type& r,
                typename std::allocator_traits<A>::size_type n)
      {

        /** Inner types **/
        // A::pointer
        requires NullablePointer<decltype (p)>;
        requires std::random_access_iterator<decltype (p)>;
        requires std::contiguous_iterator<decltype (p)>;

        // A::const_pointer
        requires NullablePointer<decltype (cp)>;
        requires std::random_access_iterator<decltype (cp)>;
        requires std::contiguous_iterator<decltype (cp)>;
        requires std::convertible_to<decltype (p), decltype (cp)>;

        // A::void_pointer
        requires NullablePointer<decltype (vp)>;
        requires std::convertible_to<decltype (p), decltype (vp)>;
        requires std::same_as<decltype (vp), typename std::allocator_traits<B>::void_pointer>;

        // A::const_void_pointer
        requires NullablePointer<decltype (vp)>;
        requires std::convertible_to<decltype (p),  decltype (cvp)>;
        requires std::convertible_to<decltype (cp), decltype (cvp)>;
        requires std::convertible_to<decltype (vp), decltype (cvp)>;
        requires std::same_as<decltype (cvp),
                              typename std::allocator_traits<B>::const_void_pointer>;

        // A::value_type
        typename A::value_type;

        // A::size_type
        requires std::unsigned_integral<decltype (n)>;

        // A::difference_type
        requires std::signed_integral<typename std::allocator_traits<A>::difference_type>;

        // A::template rebind<U>::other
        requires std::same_as<A, typename std::allocator_traits<B>::template rebind_alloc<
          typename std::allocator_traits<A>::value_type>>;

        /** Operations on pointers **/
        { *p  } -> std::same_as<typename std::allocator_traits<A>::value_type&>;
        { *cp } -> std::same_as<const typename std::allocator_traits<A>::value_type&>;
        requires std::is_pointer<decltype (p)>::value ||
          requires { { p.operator-> () } -> std::same_as<decltype (p)>; };

        requires std::is_pointer<decltype (cp)>::value ||
          requires { { cp.operator-> () } -> std::same_as<decltype (cp)>; };

        { static_cast<decltype (p)> (vp)   } -> std::same_as<decltype (p)>;
        { static_cast<decltype (cp)> (cvp) } -> std::same_as<decltype (cp)>;

        { std::pointer_traits<decltype (p)>::pointer_to (r) } -> std::same_as<decltype (p)>;

        /** Storage and lifetime operations **/
        // a.allocate (n)
        { a.allocate (n) } -> std::same_as<decltype (p)>;

        // a.allocate(n, cvp) [optional]
        requires (! requires { a.allocate (n, cvp); } ||
                    requires { { a.allocate (n, cvp) } -> std::same_as<decltype (p)>; });

        // a.deallocate(p, n)
        { a.deallocate (p, n) } -> std::convertible_to<void>;

        // a.max_size () [optional]
        requires (! requires { a.max_size (); } ||
                  requires { { a.max_size () } -> std::same_as<decltype (n)>; });

        // a.construct (xp, args) [optional]
        requires (! requires { a.construct (xp); } ||
                  requires { { a.construct (xp) } -> std::convertible_to<void>; });

        // a.destroy (xp) [optional]
        requires (! requires { a.destroy (xp); } ||
                  requires { { a.destroy (xp) } -> std::convertible_to<void>; });

        /** Relationship between instances **/
        requires NoThrowConstructibleFrom<A, decltype (b)>;
        requires NoThrowConstructibleFrom<A, decltype (std::move (b))>;

        requires BoolConstant<typename std::allocator_traits<A>::is_always_equal>;

        /** Influence on container operations **/
        // a.select_on_container_copy_construction () [optional]
        requires (! requires { a.select_on_container_copy_construction (); } ||
          requires { { a.select_on_container_copy_construction () } -> std::same_as<A>; });

        requires BoolConstant<
          typename std::allocator_traits<A>::propagate_on_container_copy_assignment>;

        requires BoolConstant<
          typename std::allocator_traits<A>::propagate_on_container_move_assignment>;

        requires BoolConstant<typename std::allocator_traits<A>::propagate_on_container_swap>;

      } &&
      requires (A a1, A a2)
      {
        { a1 == a2 } noexcept -> std::same_as<bool>;
        { a1 != a2 } noexcept -> std::same_as<bool>;
      };

    static_assert (Allocator<std::allocator<int>>);

  } // namespace concepts

#endif

  template <typename T>
  struct default_buffer_size;

  template <typename T,
            unsigned InlineCapacity = default_buffer_size<T>::value,
            typename Allocator      = std::allocator<T>>
#ifdef GCH_LIB_CONCEPTS
  requires concepts::Allocator<Allocator, T>
#endif
  class small_vector;

  template <typename T>
  struct default_buffer_size
  {
    static constexpr unsigned ideal_buffer_max = 256;
    static constexpr unsigned ideal_total      = 64;

    static constexpr std::size_t type_size = sizeof (T);
    static_assert (type_size <= ideal_buffer_max, "`sizeof(T)` too large");

    static constexpr unsigned ideal_buffer = ideal_total - sizeof (small_vector<T, 0>);
    static_assert (ideal_buffer < 64, "small_vector<T, 0> is larger than ideal_total!");

    static constexpr unsigned value = (ideal_buffer >= type_size) ? (ideal_buffer / type_size) : 1;
  };

  namespace small_vector_adl
  {

    using std::swap;

    template<typename T, typename = decltype (swap (std::declval<T&> (), std::declval<T&> ()))>
    static constexpr std::true_type test_is_swappable (int) { return { }; }

    template<typename>
    static constexpr std::false_type test_is_swappable (...) { return { }; };

    template<typename T, typename = decltype (swap (std::declval<T&> (), std::declval<T&> ()))>
    static constexpr bool test_is_nothrow_swappable (int)
    {
      return noexcept (swap (std::declval<T&> (), std::declval<T&> ()));
    }

    template<typename>
    static constexpr bool test_is_nothrow_swappable (...) { return false; };

    template <typename T>
    struct is_swappable
      : decltype (test_is_swappable<T> (0))
    { };

    template <typename T>
    struct is_nothrow_swappable
      : std::integral_constant<bool, test_is_nothrow_swappable<T> (0)>
    { };

  }

  namespace detail
  {

    template <typename T, signed InlineCapacity>
    class inline_storage
    {
    public:
      using value_t = T;
      using ptr     =       value_t *;
      using cptr    = const value_t *;

      inline_storage            (void)                      = default;
      inline_storage            (const inline_storage&)     = default;
      inline_storage            (inline_storage&&) noexcept = default;
      inline_storage& operator= (const inline_storage&)     = default;
      inline_storage& operator= (inline_storage&&) noexcept = default;
      ~inline_storage           (void)                      = default;

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      ptr
      get_inline_ptr (void) noexcept
      {
        return static_cast<ptr> (static_cast<void *> (std::addressof (*m_data)));
      }

      GCH_NODISCARD constexpr
      cptr
      get_inline_ptr (void) const noexcept
      {
        return static_cast<cptr> (static_cast<const void *> (std::addressof (*m_data)));
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
        return num_elements () * sizeof (value_t);
      }

    private:
      alignas (T) unsigned char m_data[num_bytes ()];
    };

    template <typename T>
    class GCH_EMPTY_BASE inline_storage<T, 0>
    {
    public:
      using value_t = T;
      using ptr     =       value_t *;
      using cptr    = const value_t *;

      inline_storage            (void)                      = default;
      inline_storage            (const inline_storage&)     = default;
      inline_storage            (inline_storage&&) noexcept = default;
      inline_storage& operator= (const inline_storage&)     = default;
      inline_storage& operator= (inline_storage&&) noexcept = default;
      ~inline_storage           (void)                      = default;

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      ptr
      get_inline_ptr (void) noexcept
      {
        return nullptr;
      }

      GCH_NODISCARD constexpr
      cptr
      get_inline_ptr (void) const noexcept
      {
        return nullptr;
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
                                                         && ! std::is_final<Allocator>::value
#endif // if using with C++11 just don't use an allocator marked as final
                                                         >
    class allocator_inliner;

    template <typename Allocator>
    class GCH_EMPTY_BASE allocator_inliner<Allocator, true>
      : private Allocator
    {
      using alloc_traits = std::allocator_traits<Allocator>;

      static constexpr
      bool copy_assign_is_noop = alloc_traits::propagate_on_container_copy_assignment::value;

      static constexpr
      bool move_assign_is_noop = alloc_traits::propagate_on_container_move_assignment::value;

      static constexpr
      bool swap_is_noop = alloc_traits::propagate_on_container_swap::value;

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (const allocator_inliner&) noexcept { }

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (const allocator_inliner& other)
      noexcept (noexcept (Allocator::operator= (other)))
      {
        Allocator::operator= (other);
      }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (allocator_inliner&&) noexcept { }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (allocator_inliner&& other)
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
      allocator_inliner& operator= (const allocator_inliner& other)
        noexcept (noexcept (maybe_assign (other)))
      {
        assert (&other != this && "`allocator_inliner` should not participate in "
                                  "self-copy-assignment.");
        maybe_assign (other);
        return *this;
      }

      GCH_CPP20_CONSTEXPR
      allocator_inliner& operator= (allocator_inliner&& other)
        noexcept (noexcept (maybe_assign (std::move (other))))
      {
        assert (&other != this && "`allocator_inliner` should not participate in "
                                  "self-move-assignment.");
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
      void swap (allocator_inliner&) { }

      template <bool IsNoOp = swap_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void swap (allocator_inliner& other)
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
      bool copy_assign_is_noop = alloc_traits::propagate_on_container_copy_assignment::value;

      static constexpr
      bool move_assign_is_noop = alloc_traits::propagate_on_container_move_assignment::value;

      static constexpr
      bool swap_is_noop = alloc_traits::propagate_on_container_swap::value;

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (const allocator_inliner&) noexcept { }

      template <bool IsNoOp = copy_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (const allocator_inliner& other)
        noexcept (noexcept (std::declval<decltype (other.m_alloc)&> () = other.m_alloc))
      {
        m_alloc = other.m_alloc;
      }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<IsNoOp, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (allocator_inliner&&) noexcept { }

      template <bool IsNoOp = move_assign_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void maybe_assign (allocator_inliner&& other)
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
      allocator_inliner& operator= (const allocator_inliner& other)
        noexcept (noexcept (maybe_assign (other)))
      {
        assert (&other != this && "`allocator_inliner` should not participate in "
                                  "self-copy-assignment.");
        maybe_assign (other);
        return *this;
      }

      GCH_CPP20_CONSTEXPR
      allocator_inliner& operator= (allocator_inliner&& other)
        noexcept (noexcept (maybe_assign (std::move (other))))
      {
        assert (&other != this && "`allocator_inliner` should not participate in "
                                  "self-move-assignment.");
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
      void swap (allocator_inliner&) { }

      template <bool IsNoOp = swap_is_noop,
                typename std::enable_if<! IsNoOp, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void swap (allocator_inliner& other)
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
      using alloc_base = allocator_inliner<Allocator>;

      using alloc_t = Allocator;

      using alloc_traits = std::allocator_traits<alloc_t>;

      using value_t = typename alloc_traits::value_type;
      using size_t  = typename alloc_traits::size_type;
      using diff_t  = typename alloc_traits::difference_type;
      using ptr     = typename alloc_traits::pointer;
      using cptr    = typename alloc_traits::const_pointer;

      template <typename ...>
      using void_t = void;

      template <typename, typename = void>
      struct has_ptr_traits_to_address
        : std::false_type
      { };

      template <typename P>
      struct has_ptr_traits_to_address<P,
            void_t<decltype (std::pointer_traits<P>::to_address (std::declval<P> ()))>>
        : std::true_type
      { };

      template <typename A,
                typename = decltype (std::declval<A&> ().destroy (std::declval<value_t *> ()))>
      static constexpr std::true_type test_alloc_destroy (int) { return { }; };

      static constexpr std::false_type test_alloc_destroy (...) { return { }; };

      template <typename A>
      struct has_alloc_destroy
        : decltype (test_alloc_destroy (0))
      { };

      template <typename A, typename ...Args,
                typename = decltype (std::declval<A&> ().construct (
                  std::declval<value_t *> (), std::declval<Args> ()...))>
      static constexpr std::true_type test_alloc_construct (int) { return { }; };

      template <typename ...>
      static constexpr std::false_type test_alloc_construct (...) { return { }; };

      template <typename ...Args>
      struct has_alloc_construct
        : decltype (test_alloc_construct<alloc_t, Args...> (0))
      { };

    public:
      allocator_interface            (void)                           = default;
      allocator_interface            (const allocator_interface&)     = default;
      allocator_interface            (allocator_interface&&) noexcept = default;
      allocator_interface& operator= (const allocator_interface&)     = default;
      allocator_interface& operator= (allocator_interface&&) noexcept = default;
      ~allocator_interface           (void)                           = default;

      constexpr explicit
      allocator_interface (const alloc_t& alloc)
        : alloc_base (alloc)
      { }

      static constexpr bool
      is_memcpyable_v = std::is_same<std::allocator<value_t>, alloc_t>::value &&
                        std::is_trivially_copyable<value_t>::value;

#ifdef GCH_CONTIGUOUS_ITERATOR

      template <typename InputIt>
      struct is_contiguous_iterator
        : std::integral_constant<bool,
            std::is_same<typename std::iterator_traits<InputIt>::iterator_category,
                         std::contiguous_iterator_tag>::value
#ifdef GCH_LIB_CONCEPTS
            || std::contiguous_iterator<InputIt>
#endif
            >
      { };

      template <typename InputIt>
      struct is_memcpyable_iterator
        : std::integral_constant<bool,
            is_memcpyable_v &&
            std::is_same<value_t, typename std::iterator_traits<InputIt>::value_type>::value &&
            is_contiguous_iterator<InputIt>::value>
      { };
#else
      template <typename InputIt>
      struct is_memcpyable_iterator
        : std::integral_constant<bool,
            is_memcpyable_v &&
            std::is_same<value_t, typename std::iterator_traits<InputIt>::value_type>::value &&
            (std::is_convertible<InputIt, cptr>::value ||
             std::is_convertible<InputIt, typename std::vector<value_t>::const_iterator>::value)>
      { };
#endif

      GCH_NODISCARD GCH_CPP20_CONSTEXPR
      ptr
      allocate (size_t n)
      {
        return fetch_allocator (*this).allocate (n);
      }

      GCH_CPP20_CONSTEXPR
      void deallocate (ptr p, size_t n)
      {
        fetch_allocator (*this).deallocate (to_address (p), n);
      }

      template <bool IsMemcpyable = is_memcpyable_v,
                typename std::enable_if<IsMemcpyable>::type * = nullptr>
      void construct (ptr p, const value_t& val) noexcept
      {
        std::memcpy (to_address (p), &val, sizeof (value_t));
      }

      // basically alloc_traits::construct
      template <typename ...Args,
        typename std::enable_if<! is_memcpyable_v &&
                                  has_alloc_construct<Args...>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void construct (ptr p, Args&&... args)
        noexcept (noexcept (std::declval<alloc_t&> ().construct (std::declval<value_t *> (),
                                                                 std::forward<Args> (args)...)))
      {
        fetch_allocator (*this).construct (to_address (p), std::forward<Args> (args)...);
      }

      template <typename ...Args,
        typename std::enable_if<! is_memcpyable_v &&
                                ! has_alloc_construct<Args...>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void construct (ptr p, Args&&... args)
        noexcept (noexcept (::new (std::declval<void *> ()) value_t (std::declval<Args> ()...)))
      {
        construct_at (to_address (p), std::forward<Args> (args)...);
      }

      template <bool IsMemcpyable = is_memcpyable_v,
        typename std::enable_if<IsMemcpyable>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void destroy (ptr) const noexcept { }

      template <typename A = alloc_t,
        typename std::enable_if<! is_memcpyable_v &&
                                  has_alloc_destroy<A>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void destroy (ptr p) noexcept
      {
        fetch_allocator (*this).destroy (to_address (p));
      }

      // defined so we match C++20 behavior in all cases.
      template <typename A = alloc_t,
        typename std::enable_if<! is_memcpyable_v &&
                                ! has_alloc_destroy<A>::value>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void destroy (ptr p) noexcept
      {
        destroy_at (to_address (p));
      }

      template <bool IsMemcpyable = is_memcpyable_v,
                typename std::enable_if<IsMemcpyable>::type * = nullptr>
      GCH_CPP14_CONSTEXPR
      void destroy_range (ptr, ptr) const noexcept { }

      template <bool IsMemcpyable = is_memcpyable_v,
                typename std::enable_if<! IsMemcpyable>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      void destroy_range (ptr first, ptr last) noexcept
      {
        for (; first != last; ++first)
          destroy (first);
      }

      // allowed if trivially copyable and we use the standard allocator
      // and InputIt is a contiguous iterator
      template <typename InputIt,
                typename std::enable_if<
                  is_memcpyable_iterator<InputIt>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_copy (InputIt first, InputIt last, ptr dest) noexcept
      {
        if (first != last)
          std::memcpy (to_address (dest), to_address (first), (last - first) * sizeof (value_t));
        return std::next (dest, last - first);
      }

      template <typename InputIt,
                typename std::enable_if<
                  ! is_memcpyable_iterator<InputIt>::value, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_copy (InputIt first, InputIt last, ptr d_first)
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
                typename std::enable_if<
                  std::is_trivially_constructible<V>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void uninitialized_value_construct (ptr first, ptr last)
      {
        std::fill (first, last, value_t ());
      }

      template <typename V = value_t,
                typename std::enable_if<
                  ! std::is_trivially_constructible<V>::value, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void uninitialized_value_construct (ptr first, ptr last)
      {
        ptr curr = first;
        try
        {
          for (; curr != last; ++curr)
            construct (curr);
        }
        catch (...)
        {
          destroy_range (first, curr);
          throw;
        }
      }

      template <typename V = value_t,
                typename std::enable_if<
                  std::is_trivially_default_constructible<V>::value, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void uninitialized_default_construct (ptr, ptr) { }

      template <typename V = value_t,
                typename std::enable_if<
                  ! std::is_trivially_default_constructible<V>::value, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void uninitialized_default_construct (ptr first, ptr last)
      {
        ptr curr = first;
        try
        {
          for (; curr != last; ++curr)
            ::new (static_cast<void *> (to_address (curr))) value_t;
        }
        catch (...)
        {
          destroy_range (first, curr);
          throw;
        }
      }

      template <bool IsMemcpyable = is_memcpyable_v,
                typename std::enable_if<IsMemcpyable, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      void uninitialized_fill (ptr first, ptr last, const value_t& val)
      {
        for (; first != last; ++first)
          construct (first, val);
      }

      template <bool IsMemcpyable = is_memcpyable_v,
                typename std::enable_if<! IsMemcpyable, bool>::type = false>
      GCH_CPP20_CONSTEXPR
      void uninitialized_fill (ptr first, ptr last, const value_t& val)
      {
        ptr curr = first;
        try
        {
          for (; curr != last; ++curr)
            construct (curr, val);
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
      copy_n_return_in (InputIt first, size_t count, ptr dest) noexcept
      {
        if (count != 0)
          std::memcpy (to_address (dest), to_address (first), count * sizeof (value_t));
        return std::next (first, count);
      }

      template <typename InputIt,
                typename std::enable_if<
                  ! is_memcpyable_iterator<InputIt>::value &&
                    std::is_base_of<std::random_access_iterator_tag,
                      typename std::iterator_traits<InputIt>::iterator_category>::value
                >::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      InputIt
      copy_n_return_in (InputIt first, size_t count, ptr dest)
      {
        std::copy_n (first, count, dest);
        return std::next (first, count);
      }

      template <typename InputIt,
                typename std::enable_if<
                  ! is_memcpyable_iterator<InputIt>::value &&
                  ! std::is_base_of<std::random_access_iterator_tag,
                      typename std::iterator_traits<InputIt>::iterator_category>::value
                >::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      InputIt
      copy_n_return_in (InputIt first, size_t count, ptr dest)
      {
        for (; count != 0; --count)
          *dest++ = *first++;
        return first;
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
        typename std::enable_if<! std::is_convertible<P, const value_t*>::value &&
                                  has_ptr_traits_to_address<P>::value>::type * = nullptr>
      static constexpr
      value_t *
      to_address (ptr p) noexcept
      {
        return std::pointer_traits<ptr>::to_address (p);
      }

      template <typename P = ptr,
        typename std::enable_if<! std::is_convertible<P, const value_t*>::value &&
                                  has_ptr_traits_to_address<P>::value>::type * = nullptr>
      static constexpr
      const value_t *
      to_address (cptr p) noexcept
      {
        return std::pointer_traits<cptr>::to_address (p);
      }

      template <typename Pointer,
        typename std::enable_if<! std::is_convertible<Pointer, const value_t*>::value &&
                                ! has_ptr_traits_to_address<Pointer>::value>::type * = nullptr>
      static constexpr
      auto to_address (Pointer p) noexcept
           -> decltype (to_address (p.operator-> ()))
      {
        return to_address (p.operator-> ());
      }

    private:
      template <typename U = value_t,
                typename std::enable_if<! std::is_array<U>::value>::type * = nullptr>
      static GCH_CPP20_CONSTEXPR
      void destroy_at (value_t *p) noexcept
      {
        p->~value_t ();
      }

      // replicate C++20 behavior
      template <typename U = value_t,
                typename std::enable_if<std::is_array<U>::value>::type * = nullptr>
      static GCH_CPP20_CONSTEXPR
      void destroy_at (value_t *p) noexcept
      {
        for (auto& e : *p)
          destroy_at (std::addressof (e));
      }

      template <typename ...Args>
      static GCH_CPP20_CONSTEXPR
      value_t *
      construct_at (value_t *p, Args&&... args)
        noexcept (noexcept (::new (std::declval<void *> ()) value_t (std::declval<Args> ()...)))
      {
        return ::new (const_cast<void *> (
          static_cast<const volatile void *> (p))) value_t (std::forward<Args>(args)...);
      }
    };

    static_assert (allocator_interface<std::allocator<std::size_t>>::is_memcpyable_v,
                   "std::size_t was not memcpyable");

    template class allocator_interface<std::allocator<double>>;

    template <typename Allocator, unsigned InlineCapacity>
    class small_vector_base
      : public allocator_interface<Allocator>
    {
      using alloc_interface = allocator_interface<Allocator>;

    public:
      using alloc_t = Allocator;

      using alloc_traits = std::allocator_traits<alloc_t>;

      using value_t = typename alloc_traits::value_type;
      using size_t  = typename alloc_traits::size_type;
      using diff_t  = typename alloc_traits::difference_type;
      using ptr     = typename alloc_traits::pointer;
      using cptr    = typename alloc_traits::const_pointer;

      using small_vector_t   = small_vector<value_t, InlineCapacity, Allocator>;
      using inline_storage_t = inline_storage<value_t, InlineCapacity>;

      using alloc_interface::is_memcpyable_v;
      using alloc_interface::allocate;
      using alloc_interface::deallocate;
      using alloc_interface::construct;
      using alloc_interface::destroy;
      using alloc_interface::destroy_range;
      using alloc_interface::uninitialized_copy;
      using alloc_interface::uninitialized_default_construct;
      using alloc_interface::uninitialized_value_construct;
      using alloc_interface::uninitialized_fill;
      using alloc_interface::fetch_allocator;
      using alloc_interface::to_address;

      template <typename InputIt>
      using is_memcpyable_iterator
        = typename alloc_interface::template is_memcpyable_iterator<InputIt>;

    private:
      template <typename ...>
      using void_t = void;

      template <typename AI, typename ...Args,
                typename = decltype (std::declval<AI&> ().construct (std::declval<value_t *> (),
                                                                     std::declval<Args> ()...))>
      static constexpr std::true_type test_emplace_constructible (int) { return { }; };

      template <typename ...>
      static constexpr std::false_type test_emplace_constructible (...) { return { }; };

      template <typename ...Args>
      struct is_emplace_constructible
        : decltype (test_emplace_constructible<alloc_interface, Args...> (0))
      { };

      template <typename AI, typename ...Args,
                typename = decltype (std::declval<AI&> ().construct (std::declval<value_t *> (),
                                                                     std::declval<Args> ()...))>
      static constexpr bool test_nothrow_emplace_constructible (int)
      {
        return noexcept (std::declval<AI&> ().construct (std::declval<value_t *> (),
                                                         std::declval<Args> ()...));
      }

      template <typename ...>
      static constexpr bool test_nothrow_emplace_constructible (...)
      {
        return false;
      }

      template <typename ...Args>
      struct is_nothrow_emplace_constructible
        : std::integral_constant<bool,
            test_nothrow_emplace_constructible<alloc_interface, Args...> (0)>
      { };

      template <typename AI>
      struct is_move_insertable
        : is_emplace_constructible<value_t&&>
      { };

      template <typename AI>
      struct is_nothrow_move_insertable
        : is_nothrow_emplace_constructible<value_t&&>
      { };

      static constexpr bool
      is_move_insertable_v = is_move_insertable<alloc_t>::value;

      static constexpr bool
      is_nothrow_move_insertable_v = is_nothrow_move_insertable<alloc_t>::value;

      template <typename AI>
      struct is_copy_insertable
        : std::integral_constant<bool, is_move_insertable<AI>::value &&
                                       is_emplace_constructible<value_t&>::value &&
                                       is_emplace_constructible<const value_t&>::value>
      { };



      template <typename AI>
      struct is_nothrow_copy_insertable
        : std::integral_constant<bool, is_move_insertable<AI>::value &&
                                       is_nothrow_emplace_constructible<value_t&>::value &&
                                       is_nothrow_emplace_constructible<const value_t&>::value>
      { };


      static constexpr bool
      is_copy_insertable_v = is_copy_insertable<alloc_t>::value;

      static constexpr bool
      is_nothrow_copy_insertable_v = is_nothrow_copy_insertable<alloc_t>::value;

      template <typename AI, typename Enable = void>
      struct is_eraseable
        : std::false_type
      { };

      template <typename AI>
      struct is_eraseable<AI,
            void_t<decltype (std::declval<AI&> ().destroy (std::declval<value_t *> ()))>>
        : std::true_type
      { };

      static constexpr bool is_eraseable_v = is_eraseable<alloc_t>::value;

      using relocating_ptr = typename std::conditional<
        ! std::is_nothrow_move_constructible<value_t>::value &&
          std::is_copy_constructible<value_t>::value,
        ptr,
        std::move_iterator<ptr>>::type;

    public:
      template <typename Ptr>
      struct pointer_traits
      {
        using pointer = Ptr;
      };

      small_vector_base            (void)                         = default;
      small_vector_base            (const small_vector_base&)     = delete;
      small_vector_base            (small_vector_base&&) noexcept = delete;
      GCH_CPP20_CONSTEXPR small_vector_base& operator= (const small_vector_base&);
      GCH_CPP20_CONSTEXPR small_vector_base& operator= (small_vector_base&&) noexcept;
//    ~small_vector_base           (void)                         = impl;

      GCH_CPP20_CONSTEXPR
      small_vector_base (const small_vector_base& other, const alloc_t& alloc)
        : alloc_interface (alloc),
          m_data_ptr (other.using_inline_storage () ? get_storage_ptr ()
                                                    : allocate (other.get_capacity ())),
          m_current_capacity (other.m_current_capacity),
          m_current_size     (other.m_current_size)
      {
        try
        {
          uninitialized_copy (other.get_begin_ptr (), other.get_end_ptr (), get_data_ptr ());
        }
        catch (...)
        {
          if (! using_inline_storage ())
            deallocate (get_begin_ptr (), get_capacity ());
          throw;
        }
      }

      // noexcept if (! other.is_inline_storage)
      // otherwise std::is_nothrow_move_constructible_v<value_t>
      template <typename InAlloc>
      GCH_CPP20_CONSTEXPR
      small_vector_base (small_vector_base&& other, InAlloc alloc)
            noexcept (std::is_nothrow_move_constructible<value_t>::value)
        : alloc_interface (std::forward<InAlloc> (alloc)),
          m_data_ptr (other.using_inline_storage () ? get_storage_ptr () : other.m_data_ptr),
          m_current_capacity (other.m_current_capacity),
          m_current_size     (other.m_current_size)
      {
        if (other.using_inline_storage ())
          uninitialized_move (other.get_begin_ptr (), other.get_end_ptr (), get_data_ptr ());

#ifndef NDEBUG
        other.m_data_ptr = other.get_storage_ptr ();
#endif
        other.m_current_capacity = 0;
        other.m_current_size     = 0;
      }

      constexpr explicit
      small_vector_base (const alloc_t& alloc)
        : alloc_interface (alloc)
      { }

      GCH_CPP20_CONSTEXPR
      small_vector_base (size_t count, const alloc_t& alloc)
        : alloc_interface (alloc),
          m_data_ptr         ((count <= InlineCapacity) ? get_storage_ptr () : allocate (count)),
          m_current_capacity ((count <= InlineCapacity) ? InlineCapacity     : count),
          m_current_size     (count)
      {
        try
        {
          uninitialized_value_construct (get_begin_ptr (), get_end_ptr ());
        }
        catch (...)
        {
          if (! using_inline_storage ())
            deallocate (get_begin_ptr (), get_capacity ());
          throw;
        }
      }

      GCH_CPP20_CONSTEXPR
      small_vector_base (size_t count, const value_t& val,  const alloc_t& alloc)
        : alloc_interface (alloc),
          m_data_ptr         ((count <= InlineCapacity) ? get_storage_ptr () : allocate (count)),
          m_current_capacity ((count <= InlineCapacity) ? InlineCapacity     : count),
          m_current_size     (count)
      {
        try
        {
          uninitialized_fill (get_begin_ptr (), get_begin_ptr () + count, val);
        }
        catch (...)
        {
          if (! using_inline_storage ())
            deallocate (get_begin_ptr (), get_capacity ());
          throw;
        }
      }

      template <typename InputIt>
      GCH_CPP20_CONSTEXPR
      small_vector_base (InputIt first, InputIt last, size_t count, const alloc_t& alloc)
        : alloc_interface (alloc),
          m_data_ptr         ((count <= InlineCapacity) ? get_storage_ptr () : allocate (count)),
          m_current_capacity ((count <= InlineCapacity) ? InlineCapacity     : count),
          m_current_size     (count)
      {
        try
        {
          uninitialized_copy(first, last, get_begin_ptr ());
        }
        catch (...)
        {
          if (! using_inline_storage ())
            deallocate (get_begin_ptr (), get_capacity ());
          throw;
        }
      }

      GCH_CPP20_CONSTEXPR
      ~small_vector_base (void) noexcept
      {
        destroy_range (get_begin_ptr (), get_end_ptr ());
        if (! using_inline_storage () && get_size () != 0)
          deallocate (get_begin_ptr (), get_capacity ());
      }

      GCH_CPP20_CONSTEXPR
      void replace_alloc (size_t count, const value_t& val)
      {
        // precondition: capacity () < count
        assert (get_capacity () < count && "capacity () was greater than count");

        size_t new_capacity = calculate_new_capacity (count);
        ptr    new_begin    = allocate (new_capacity);

        try
        {
          uninitialized_fill (new_begin, new_begin + count, val);
        }
        catch (...)
        {
          deallocate (new_begin, new_capacity);
          throw;
        }

        destroy_range (get_begin_ptr (), get_end_ptr ());
        deallocate (get_begin_ptr (), get_capacity ());

        m_data_ptr         = new_begin;
        m_current_capacity = new_capacity;
        m_current_size     = count;
      }

      template <typename InputIt>
      GCH_CPP20_CONSTEXPR
      void replace_alloc (InputIt first, InputIt last, size_t count)
      {
        // precondition: capacity () < count
        assert (get_capacity () < count && "capacity () was greater than count");

        size_t new_capacity = calculate_new_capacity (count);
        ptr    new_begin    = allocate (new_capacity);

        try
        {
          uninitialized_copy (first, last, new_begin);
        }
        catch (...)
        {
          deallocate (new_begin, new_capacity);
          throw;
        }

        destroy_range (get_begin_ptr (), get_end_ptr ());
        deallocate (get_begin_ptr (), get_capacity ());

        m_data_ptr         = new_begin;
        m_current_capacity = new_capacity;
        m_current_size     = count;
      }

      GCH_NODISCARD constexpr
      size_t
      get_size (void) const noexcept
      {
        return m_current_size;
      }

      GCH_NODISCARD constexpr
      bool
      is_empty (void) const noexcept
      {
        return get_size () == 0;
      }

      GCH_NODISCARD constexpr
      size_t
      get_capacity (void) const noexcept
      {
        return m_current_capacity;
      }

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      size_t
      get_max_size (void) const noexcept
      {
        // protected from max/min macros
        return (std::min<size_t>) (alloc_traits::max_size (fetch_allocator (*this)),
                                   (std::numeric_limits<diff_t>::max) ());
      }

      GCH_NODISCARD constexpr
      ptr
      get_data_ptr (void) const noexcept
      {
        return m_data_ptr;
      }

      GCH_NODISCARD constexpr
      ptr
      get_begin_ptr (void) const noexcept
      {
        return get_data_ptr ();
      }

      GCH_NODISCARD constexpr
      ptr
      get_end_ptr (void) const noexcept
      {
        return get_begin_ptr () + get_size ();
      }

      GCH_NODISCARD constexpr
      ptr
      get_uninitialized_end_ptr (void) const noexcept
      {
        return get_begin_ptr () + get_capacity ();
      }

      GCH_NODISCARD constexpr
      alloc_t
      copy_allocator (void) const noexcept
      {
        return alloc_t (fetch_allocator (*this));
      }

      GCH_CPP14_CONSTEXPR
      ptr
      get_storage_ptr (void) noexcept
      {
        return as_storage (*this).get_inline_ptr ();
      }

      GCH_NODISCARD constexpr
      cptr
      get_storage_ptr (void) const noexcept
      {
        return as_storage (*this).get_inline_ptr ();
      }

      GCH_NODISCARD constexpr
      bool
      using_inline_storage (void) const noexcept
      {
        return get_storage_ptr () == get_data_ptr ();
      }

      GCH_NODISCARD constexpr
      bool
      is_inlinable (void) const noexcept
      {
        return get_size () <= InlineCapacity;
      }

      GCH_CPP14_CONSTEXPR
      bool
      is_valid_after_resize (cptr pos, size_t new_size)
      {
        assert (! is_uninitialized_element (pos) &&
                "Element is within the uninitialized range [`end ()`, `begin () + capacity ()`) "
                "and cannot be dereferenced.");

        // outside of range
        if (pos < get_begin_ptr () || get_end_ptr () <= pos)
          return true;

        if (new_size <= get_size ())
          return pos < (get_begin_ptr () + new_size);

        return new_size <= get_capacity ();
      }

      constexpr
      bool
      is_element (cptr pos) const noexcept
      {
        return get_begin_ptr () <= pos && pos < get_end_ptr ();
      }

      constexpr
      bool
      is_uninitialized_element (cptr pos) const noexcept
      {
        return get_end_ptr () <= pos && pos < get_uninitialized_end_ptr ();
      }

      constexpr
      bool
      is_overlapping_range (cptr first, cptr last) const noexcept
      {
        return assert (first < last && "Invalid range."),
               ! (last < get_begin_ptr () || get_uninitialized_end_ptr () <= first);
      }

      GCH_NODISCARD GCH_CPP14_CONSTEXPR
      size_t
      calculate_new_capacity (const size_t new_size) const noexcept
      {
        size_t capacity = get_capacity ();

        if (get_max_size () - (capacity / 2) < capacity)
          return get_max_size ();

        // this growth factor actually sucks...
        // size_t new_capacity = capacity + (capacity / 2);

        size_t new_capacity = 2 * capacity;
        if (new_capacity < new_size)
          return new_size;
        return new_capacity;
      }

      GCH_NORETURN static GCH_CPP20_CONSTEXPR
      void throw_range_error (void)
      {
        throw std::out_of_range ("The requested index was out of range.");
      }

      GCH_NORETURN static GCH_CPP20_CONSTEXPR
      void throw_length_error (void)
      {
        throw std::length_error ("Required allocation exceeds the maximum size.");
      }

      GCH_CPP14_CONSTEXPR
      void set_data_ptr (ptr p) noexcept
      {
        m_data_ptr = p;
      }

      GCH_CPP14_CONSTEXPR
      void set_size (size_t new_size) noexcept
      {
        m_current_size = new_size;
      }

      GCH_CPP14_CONSTEXPR
      void set_capacity (size_t new_capacity) noexcept
      {
        m_current_size = new_capacity;
      }

      template <bool IsNoThrowMoveInsertable = is_nothrow_move_insertable_v,
                typename std::enable_if<IsNoThrowMoveInsertable &&
                                        is_memcpyable_v>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_move (ptr first, ptr last, ptr dest) noexcept
      {
        // strong exception guarantee
        return uninitialized_copy (first, last, dest);
      }

      template <bool IsNoThrowMoveInsertable = is_nothrow_move_insertable_v,
                typename std::enable_if<IsNoThrowMoveInsertable &&
                                        ! is_memcpyable_v>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_move (ptr first, ptr last, ptr d_first) noexcept
      {
        // strong exception guarantee
        ptr d_last = d_first;
        for (; first != last; ++first, static_cast<void> (++d_last))
          construct (d_last, std::move (*first));
        return d_last;
      }

      template <bool IsNoThrowMoveInsertable = is_nothrow_move_insertable_v,
                typename std::enable_if<! IsNoThrowMoveInsertable>::type * = nullptr>
      GCH_CPP20_CONSTEXPR
      ptr
      uninitialized_move (ptr first, ptr last, ptr dest)
      {
        // if (is_nothrow_move_insertable_v)
        //   we don't use this overload
        // else if (is_copy_constructible_v)
        //   uses noexcept(?)     copy constructor ;    strong exception guarantee
        // else if (  is_move_constructible_v && ! is_copy_constructible_v)
        //   uses noexcept(false) move constructor ; no strong exception guarantee
        // else if (! is_move_constructible_v && ! is_copy_constructible_v)
        //   ill-formed
        return uninitialized_copy (relocating_ptr (first), relocating_ptr (last), dest);
      }

      template <bool IsMemcpyable = is_memcpyable_v,
                typename std::enable_if<IsMemcpyable, bool>::type = true>
      GCH_CPP20_CONSTEXPR
      ptr shift_into_uninitialized (ptr first, ptr last, size_t shift)
      {
        // shift elements over to the right into uninitialized space
        return static_cast<ptr> (std::memmove (to_address (first + shift), to_address (first),
                                               (last - first) * sizeof (value_t)));
      }

      template <bool IsMemcpyable = is_memcpyable_v,
                typename std::enable_if<! IsMemcpyable, bool>::type = false>
      ptr shift_into_uninitialized (ptr first, ptr last, size_t shift)
      {
        // shift elements over to the right into uninitialized space
        uninitialized_move (last - shift, last, last);
        return std::move_backward (first, last - shift, last);
      }

      GCH_CPP20_CONSTEXPR
      ptr
      append (size_t count, const value_t& val)
      {
        ptr ret = request_elements_end (count);
        uninitialized_fill (ret, ret + count, val);
        set_size (get_size () + count);
        return ret;
      }

      template <typename InputIt>
      GCH_CPP20_CONSTEXPR
      ptr
      append_range (InputIt first, InputIt last)
      {
        size_t num_insert = std::distance (first, last);
        ptr ret = request_elements_end (num_insert);
        uninitialized_copy (first, last, ret);
        set_size (get_size () + num_insert);
        return ret;
      }

      GCH_CPP20_CONSTEXPR ptr request_elements (ptr pos, size_t num);
      GCH_CPP20_CONSTEXPR ptr do_shrink_to_fit (void);

      GCH_CPP20_CONSTEXPR
      ptr
      request_capacity (size_t request)
      {
        if (get_capacity () < request)
          reallocate (calculate_new_capacity (request));
        return get_end_ptr ();
      }

      GCH_CPP20_CONSTEXPR
      ptr
      request_elements_end (size_t num)
      {
        if (get_max_size () - get_size () < num)
          throw_length_error ();
        return request_capacity (get_size () + num);
      }

      template <typename V = value_t>
      GCH_CPP20_CONSTEXPR
      typename std::enable_if<small_vector_adl::is_swappable<V>::value &&
                              std::is_move_constructible<V>::value>::type
      swap (small_vector_base& other)
        noexcept ((std::allocator_traits<alloc_t>::propagate_on_container_swap::value
#ifdef GCH_LIB_IS_ALWAYS_EQUAL
                   || std::allocator_traits<alloc_t>::is_always_equal::value
#endif
                  ) &&
                  gch::small_vector_adl::is_nothrow_swappable<value_t>::value &&
                  std::is_nothrow_move_constructible<value_t>::value);

    private:
      // returns end of new allocation
      GCH_CPP20_CONSTEXPR void reallocate (size_t new_capacity);

      static constexpr
      inline_storage<value_t, InlineCapacity>&
      as_storage (small_vector_base& self) noexcept;

      static constexpr
      const inline_storage<value_t, InlineCapacity>&
      as_storage (const small_vector_base& self) noexcept;

      ptr    m_data_ptr         = get_storage_ptr ();
      size_t m_current_capacity = InlineCapacity;
      size_t m_current_size     = 0;
    };

    /* some implementations to dissuade the compiler from inlining */

    // dissuade from inline
    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    small_vector_base<Allocator, InlineCapacity>&
    small_vector_base<Allocator, InlineCapacity>::operator= (const small_vector_base& other)
    {
      assert (&other != this && "`small_vector` private base `small_vector_base` should not "
                                "participate in self-copy-assignment.");

      alloc_interface::operator= (other);

      if (get_capacity () < other.get_size ())
      {
        size_t new_capacity = calculate_new_capacity (other.get_size ());
        ptr new_begin = allocate (new_capacity);
        uninitialized_copy (other.get_begin_ptr (), other.get_end_ptr (), new_begin);
        destroy_range (get_begin_ptr (), get_end_ptr ());

        if (! using_inline_storage ())
          deallocate (get_begin_ptr (), get_capacity ());

        m_data_ptr     = new_begin;
        m_current_size = new_capacity;

      }
      else if (get_size () < other.get_size ())
      {
        std::copy_n (other.get_begin_ptr (), get_size (), get_begin_ptr ());
        uninitialized_copy (other.get_begin_ptr () + get_size (), other.get_end_ptr(),
                            get_end_ptr ());
      }
      else
        destroy_range (std::copy (other.get_begin_ptr (), other.get_end_ptr (), get_begin_ptr ()),
                       get_end_ptr ());
      m_current_size = other.m_current_size;
      return *this;
    }

    // dissuade from inline
    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    small_vector_base<Allocator, InlineCapacity>&
    small_vector_base<Allocator, InlineCapacity>::operator= (small_vector_base&& other) noexcept
    {
      assert (&other != this && "`small_vector` private base `small_vector_base` should not "
                                "participate in self-move-assignment.");

      // static_cast instead of move to make clang-tidy shut up
      alloc_interface::operator= (static_cast<alloc_interface&&> (other));

      if (other.using_inline_storage () || (using_inline_storage () && other.is_inlinable ()))
      {
        if (get_size () < other.get_size ())
        {
          std::copy_n (std::make_move_iterator (other.get_storage_ptr ()), get_size (),
                       get_begin_ptr ());
          uninitialized_move (other.get_storage_ptr () + get_size (), other.get_end_ptr (),
                              get_begin_ptr () + get_size ());
        }
        else
        {
          std::copy_n (std::make_move_iterator (other.get_storage_ptr ()), other.get_size (),
                       get_begin_ptr ());
          destroy_range (get_begin_ptr () + other.get_size (), get_end_ptr ());
        }

        // capacity does not change in this case
        m_current_size = other.m_current_size;
      }
      else
      {
        m_data_ptr         = other.m_data_ptr;
        m_current_capacity = other.m_current_capacity;
        m_current_size     = other.m_current_size;

#ifndef NDEBUG
        other.m_data_ptr = other.get_storage_ptr ();
#endif
        other.m_current_capacity = 0;
        other.m_current_size     = 0;
      }

      if (get_capacity () < other.get_size ())
      {
        size_t new_capacity = calculate_new_capacity (other.get_size ());
        ptr new_begin = allocate (new_capacity);
        uninitialized_copy (other.get_begin_ptr (), other.get_end_ptr (), new_begin);
        destroy_range (get_begin_ptr (), get_end_ptr ());

        if (! using_inline_storage ())
          deallocate (get_begin_ptr (), get_capacity ());

        m_data_ptr     = new_begin;
        m_current_size = new_capacity;

      }
      else if (get_size () < other.get_size ())
      {
        std::copy_n (other.get_begin_ptr (), get_size (), get_begin_ptr ());
        uninitialized_copy (other.get_begin_ptr () + get_size (), other.get_end_ptr(),
                            get_end_ptr ());
      }
      else
        destroy_range (std::copy (other.get_begin_ptr (), other.get_end_ptr (), get_begin_ptr ()),
                       get_end_ptr ());
      m_current_size = other.m_current_size;
      return *this;
    }

    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    typename small_vector_base<Allocator, InlineCapacity>::ptr
    small_vector_base<Allocator, InlineCapacity>::request_elements (ptr pos, size_t num)
    {
      if (pos == get_end_ptr ())
        return request_elements_end (num);

      if (get_max_size () - get_size () < num)
        throw_length_error ();

      size_t new_size = get_size () + num;
      if (new_size <= get_capacity ())
      {
        shift_into_uninitialized (pos, get_end_ptr (), num);
        return pos;
      }

      size_t new_capacity = calculate_new_capacity (new_size);
      ptr    new_begin    = allocate (new_capacity);

      // move everything before `pos`
      ptr ret = uninitialized_move (get_begin_ptr (), pos, new_begin);

      // move the range after `pos` to the new offset range
      uninitialized_move (pos, get_end_ptr (), ret + num);

      destroy_range (get_begin_ptr (), get_end_ptr ());
      if (! using_inline_storage ())
        deallocate (get_begin_ptr (), get_capacity ());

      m_data_ptr         = new_begin;
      m_current_capacity = new_capacity;

      return ret;
    }

    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    typename small_vector_base<Allocator, InlineCapacity>::ptr
    small_vector_base<Allocator, InlineCapacity>::do_shrink_to_fit (void)
    {
      if (using_inline_storage () || get_size () == get_capacity ())
        return get_begin_ptr ();

      size_t new_capacity = get_size ();

      ptr new_begin = allocate (new_capacity);
      ptr ret = uninitialized_move (get_begin_ptr (), get_end_ptr (), new_begin);

      destroy_range (get_begin_ptr (), get_end_ptr ());
      deallocate (get_begin_ptr (), get_capacity ());

      m_data_ptr         = new_begin;
      m_current_capacity = new_capacity;

      assert (get_size () == get_capacity ());

      return ret;
    }

    template <typename Allocator, unsigned InlineCapacity>
    GCH_CPP20_CONSTEXPR
    void
    small_vector_base<Allocator, InlineCapacity>::reallocate (size_t new_capacity)
    {
      ptr new_begin = allocate (new_capacity);

      try
      {
        uninitialized_move (get_begin_ptr (), get_end_ptr (), new_begin);
      }
      catch (...)
      {
        deallocate (new_begin, new_capacity);
        throw;
      }
      destroy_range (get_begin_ptr (), get_end_ptr ());

      if (! using_inline_storage ())
        deallocate (get_begin_ptr (), get_capacity ());

      m_data_ptr         = new_begin;
      m_current_capacity = new_capacity;
    }

    template <typename Allocator, unsigned InlineCapacity>
    template <typename V>
    GCH_CPP20_CONSTEXPR
    typename std::enable_if<small_vector_adl::is_swappable<V>::value &&
                            std::is_move_constructible<V>::value>::type
    small_vector_base<Allocator, InlineCapacity>::swap (small_vector_base& other)
      noexcept ((std::allocator_traits<alloc_t>::propagate_on_container_swap::value
#ifdef GCH_LIB_IS_ALWAYS_EQUAL
                 || std::allocator_traits<alloc_t>::is_always_equal::value
#endif
                ) &&
                gch::small_vector_adl::is_nothrow_swappable<value_t>::value &&
                std::is_nothrow_move_constructible<value_t>::value)
    {
      // note: no-op for std::allocator
      alloc_interface::swap (other);

      using std::swap;

      if (using_inline_storage ())
      {
        if (other.using_inline_storage ())
        {
          std::swap_ranges (get_begin_ptr (), get_end_ptr (), other.get_begin_ptr ());
          // m_data_ptr still equal to get_storage_ptr ()
          // m_current_capacity still equal to InlineCapacity
        }
        else
        {
          uninitialized_move (get_begin_ptr (), get_end_ptr (), other.get_storage_ptr ());
          m_data_ptr         = other.m_data_ptr;
          m_current_capacity = other.m_current_capacity;
        }

        other.m_data_ptr         = other.get_storage_ptr ();
        other.m_current_capacity = InlineCapacity;
      }
      else if (other.using_inline_storage ())
      {
        // other is inline
        // *this is not
        uninitialized_move (other.get_begin_ptr (), other.get_end_ptr (), get_storage_ptr ());
        other.m_data_ptr         = m_data_ptr;
        other.m_current_capacity = m_current_capacity;

        m_data_ptr         = get_storage_ptr ();
        m_current_capacity = InlineCapacity;
      }
      else
      {
        // neither are inline so just swap
        swap (m_data_ptr,         other.m_data_ptr);
        swap (m_current_capacity, other.m_current_capacity);
      }
      swap (m_current_size, other.m_current_size);
    }

  } // detail

  template <typename T, unsigned InlineCapacity, typename Allocator>
#ifdef GCH_LIB_CONCEPTS
  requires concepts::Allocator<Allocator, T>
#endif
  class small_vector
    : private detail::small_vector_base<
        typename std::allocator_traits<Allocator>::template rebind_alloc<T>, InlineCapacity>,
      private detail::inline_storage<T, InlineCapacity>
  {
  public:
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = typename std::allocator_traits<allocator_type>::size_type;
    using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
    using reference       =       value_type&;
    using const_reference = const value_type&;
    using pointer         = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;

    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  private:
    using rebound_alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;

    using base = detail::small_vector_base<rebound_alloc, InlineCapacity>;
    using storage_base  = detail::inline_storage<T, InlineCapacity>;

    // inline_storage inherited here because
    // 1. it needs to be after all other members for space optimization - so it has
    //    to either be here or after the members of small_vector_base
    // 2. empty base optimization - so it shouldn't be a member of small_vector_base

    friend detail::small_vector_base<rebound_alloc, InlineCapacity>;

    static_assert (std::is_trivially_default_constructible<storage_base>::value,
                   "inline storage was not trivially default constructible");

    template <typename It, typename Enable = void>
    struct is_input_iterator
      : std::false_type
    { };

    template <typename It>
    struct is_input_iterator<It,
                             typename std::enable_if<std::is_base_of<
                               std::input_iterator_tag,
                               typename std::iterator_traits<It>::iterator_category>::value>::type>
      : std::true_type
    { };

  public:
    /* constructors */
    GCH_CPP20_CONSTEXPR
    small_vector (void) noexcept (noexcept (allocator_type ())) = default;


    GCH_CPP20_CONSTEXPR explicit
    small_vector (const small_vector& other)
      : base (other,
              std::allocator_traits<allocator_type>::select_on_container_copy_construction (
                other.get_allocator ()))
    { }

    GCH_CPP20_CONSTEXPR
    small_vector (small_vector&& other) noexcept
      : base (std::move (other), std::move (base::fetch_allocator (other)))
    { }

    GCH_CPP20_CONSTEXPR explicit
    small_vector (const allocator_type& alloc) noexcept
      : base (alloc)
    { }

    GCH_CPP20_CONSTEXPR
    small_vector (const small_vector& other, const allocator_type& alloc)
      : base (other, alloc)
    { }

    GCH_CPP20_CONSTEXPR
    small_vector (small_vector&& other, const allocator_type& alloc)
      : base (std::move (other), alloc)
    { }

    GCH_CPP20_CONSTEXPR explicit
    small_vector (size_type count, const allocator_type& alloc = allocator_type ())
      : base (count, alloc)
    { }

    GCH_CPP20_CONSTEXPR
    small_vector (std::initializer_list<value_type> init,
                  const allocator_type& alloc = allocator_type ())
      : small_vector (init.begin (), init.end (), alloc)
    { }

    GCH_CPP20_CONSTEXPR explicit
    small_vector (size_type count, const_reference value,
                  const allocator_type& alloc = allocator_type ())
      : base (count, value, alloc)
    { }

    template <typename InputIt,
              typename = typename std::enable_if<is_input_iterator<InputIt>::value>::type>
    GCH_CPP20_CONSTEXPR
    small_vector (InputIt first, InputIt last, const allocator_type& alloc = allocator_type ())
      : base (first, last, std::distance (first, last), alloc)
    { }

    /* destructor */
    GCH_CPP20_CONSTEXPR
    ~small_vector (void) = default;

    /* assignment */
    GCH_CPP20_CONSTEXPR
    small_vector&
    operator= (const small_vector& other)
    {
      if (&other != this)
        base::operator= (other);
      return *this;
    }

    GCH_CPP20_CONSTEXPR
    small_vector&
    operator= (small_vector&& other) noexcept
    {
      if (&other != this)
        base::operator= (std::move (other));
      return *this;
    }

    GCH_CPP20_CONSTEXPR
    small_vector&
    operator= (std::initializer_list<value_type> ilist)
    {
      assign (ilist);
      return *this;
    }

    GCH_CPP20_CONSTEXPR
    void
    assign (size_type count, const_reference value)
    {
      if (capacity () < count)
        return base::replace_alloc (count, value);
      else if (size () < count)
      {
        std::fill (begin (), end (), value);
        base::uninitialized_fill (end (), begin () + count, value);
      }
      else
        erase (std::fill_n (begin (), count, value), end ());
      base::set_size (count);
    }

    GCH_CPP20_CONSTEXPR
    void
    assign (std::initializer_list<value_type> ilist)
    {
      assign (ilist.begin (), ilist.end ());
    }

    template <typename InputIt,
              typename = typename std::enable_if<is_input_iterator<InputIt>::value>::type>
    GCH_CPP20_CONSTEXPR
    void
    assign (InputIt first, InputIt last)
    {
      assert (! base::is_overlapping_range (first, last) &&
                "The range overlaps with the vector and will be invalidated during assignment.");
      size_type count = std::distance (first, last);
      if (capacity () < count)
        return base::replace_alloc (first, last, count);
      else if (size () < count)
        base::uninitialized_copy (base::copy_n_return_in (first, count, begin ()), last, end ());
      else
        erase (std::copy (first, last, begin ()), end ());
      base::set_size (count);
    }

    /* swap */
    // note: dispariate with the standard
    template <typename ValueType = value_type,
      typename = typename std::enable_if<small_vector_adl::is_swappable<ValueType>::value &&
                                         std::is_move_constructible<ValueType>::value>::type>
    GCH_CPP20_CONSTEXPR
    void
    swap (small_vector& other)
      noexcept ((std::allocator_traits<allocator_type>::propagate_on_container_swap::value
#ifdef GCH_LIB_IS_ALWAYS_EQUAL
                 || std::allocator_traits<allocator_type>::is_always_equal::value
#endif
                ) &&
                small_vector_adl::is_nothrow_swappable<value_type>::value &&
                std::is_nothrow_move_constructible<value_type>::value)
    {
      // base handles the storage swap if needed
      base::swap (other);
    }

    /* iteration */
    GCH_CPP14_CONSTEXPR
    iterator
    begin (void) noexcept
    {
      return base::get_begin_ptr ();
    }

    constexpr
    const_iterator
    begin (void) const noexcept
    {
      return base::get_begin_ptr ();
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
      return base::get_end_ptr ();
    }

    constexpr
    const_iterator
    end (void) const noexcept
    {
      return base::get_end_ptr ();
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
        base::throw_range_error ();
      return begin ()[pos];
    }

    GCH_CPP14_CONSTEXPR
    const_reference
    at (size_type pos) const
    {
      if (size () <= pos)
        base::throw_range_error ();
      return begin ()[pos];
    }

    GCH_CPP14_CONSTEXPR
    reference
    operator[] (size_type pos) noexcept
    {
      return begin ()[pos];
    }

    constexpr
    const_reference
    operator[] (size_type pos) const noexcept
    {
      return begin ()[pos];
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
      return base::get_begin_ptr ();
    }

    constexpr
    const_pointer
    data (void) const noexcept
    {
      return base::get_begin_ptr ();
    }

    /* state information */
    constexpr
    size_type
    size (void) const noexcept
    {
      return base::get_size ();
    }

    GCH_NODISCARD constexpr bool
    empty (void) const noexcept
    {
      return size () == 0;
    }

    GCH_CPP14_CONSTEXPR
    size_type
    max_size (void) const noexcept
    {
      return base::get_max_size ();
    }

    constexpr
    size_type
    capacity (void) const noexcept
    {
      return base::get_capacity ();
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
      requires concepts::CopyAssignable<value_type> &&
               concepts::CopyInsertable<value_type, small_vector, allocator_type>
#endif
    {
      if (pos == cend ())
      {
        push_back (value);
        return std::prev (end ());
      }

      assert (! base::is_element (std::addressof (value)) &&
                "The argument `value` is an element of `*this`.");

      pointer resolved_pos = base::request_elements (begin () + (pos - cbegin ()), 1);
      base::construct (resolved_pos, value);
      base::set_size (size () + 1);
      return resolved_pos;
    }

    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, value_type&& value)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::MoveAssignable<value_type> &&
               concepts::MoveInsertable<value_type, small_vector, allocator_type>
#endif
    {
      if (pos == cend ())
      {
        push_back (std::move (value));
        return std::prev (end ());
      }

      assert (! base::is_element (std::addressof (value)) &&
                "The argument `value` is an element of `*this`.");

      pointer resolved_pos = base::request_elements (begin () + (pos - cbegin ()), 1);
      base::construct (resolved_pos, std::move (value));
      base::set_size (size () + 1);
      return resolved_pos;
    }

    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, size_type count, const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::CopyAssignable<value_type> &&
               concepts::CopyInsertable<value_type, small_vector, allocator_type>
#endif
    {
      if (count == 0)
        return begin () + (pos - cbegin ());

      assert (! base::is_element (std::addressof (value)) &&
                "The argument `value` is an element of `*this`.");

      if (pos == end ())
        return base::append (count, value);

      const iterator resolved_pos = base::request_elements (begin () + (pos - cbegin ()), count);

      if (resolved_pos == pos)
      {
        // we didn't realloc, so overwrite until we reach the end, then uninitialized_copy the rest
        if (resolved_pos + count <= end ())
        {
          // totally within the previously constructed range
          std::fill (resolved_pos, resolved_pos + count, value);
        }
        else
        {
          std::fill (resolved_pos, end (), value);
          base::uninitialized_fill (end (), resolved_pos + count, value);
        }
      }
      else
        base::uninitialized_fill (resolved_pos, resolved_pos + count, value);

      base::set_size (size () + count);
      return resolved_pos;
    }

    template <typename InputIt>
    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, InputIt first, InputIt last)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::Swappable<value_type> &&
               concepts::MoveAssignable<value_type> &&
               concepts::MoveConstructible<value_type> &&
               concepts::MoveInsertable<value_type, small_vector, allocator_type> &&
               concepts::EmplaceConstructible<value_type, small_vector, allocator_type,
                                              decltype (*first)>
#endif
    {
      if (first == last)
        return begin () + (pos - cbegin ());

      assert (! base::is_overlapping_range (first, last) &&
                "The input range overlaps with the range of `*this`.");

      if (pos == end ())
        return base::append_range (first, last);

      // TODO: make exception safe
      const size_type num_insert = std::distance (first, last);
      const iterator resolved_pos = base::request_elements (begin () + (pos - cbegin ()),
                                                            num_insert);

      if (resolved_pos == pos)
      {
        // we didn't realloc, so overwrite until we reach the end, then uninitialized_copy the rest
        if (resolved_pos + num_insert <= end ())
        {
          // totally within the previously constructed range
          std::copy (first, last, resolved_pos);
        }
        else
        {
          InputIt pivot = base::copy_n_return_in (first, end () - resolved_pos, resolved_pos);
          base::uninitialized_copy (pivot, last, end ());
        }
      }
      else
        base::uninitialized_copy (first, last, resolved_pos);

      base::set_size (size () + num_insert);
      return resolved_pos;
    }


    GCH_CPP20_CONSTEXPR
    iterator
    insert (const_iterator pos, std::initializer_list<value_type> ilist)
#ifdef GCH_LIB_CONCEPTS
      requires (concepts::Swappable<value_type> &&
                concepts::MoveAssignable<value_type> &&
                concepts::MoveConstructible<value_type> &&
                concepts::MoveInsertable<value_type, small_vector, allocator_type> &&
                concepts::EmplaceConstructible<value_type, small_vector, allocator_type,
                                               decltype (*std::begin (ilist))>)
#endif
    {
      return insert (pos, ilist.begin (), ilist.end ());
    }

    template <typename ...Args>
    GCH_CPP20_CONSTEXPR
    iterator
    emplace (const_iterator pos, Args&&... args)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::MoveAssignable<value_type> &&
               concepts::MoveInsertable<value_type, small_vector, allocator_type> &&
               concepts::EmplaceConstructible<value_type, small_vector, allocator_type, Args...>
#endif
    {
      if (pos == cend ())
      {
        emplace_back (std::forward<Args> (args)...);
        return std::prev (cend ());
      }
      iterator resolved_pos = base::request_elements (begin () + (pos - cbegin ()), 1);
      // pos now may be invalidated

      base::construct (resolved_pos, std::forward<Args> (args)...);
      base::set_size (size () + 1);
      return resolved_pos;
    }

    GCH_CPP20_CONSTEXPR
    iterator
    erase (const_iterator pos)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::MoveAssignable<value_type>
#endif
    {
      assert (begin () <= pos    && "The argument `pos` is out of bounds (before `begin ()`)."   );
      assert (pos      <  end () && "The argument `pos` is out of bounds (at or after `end ()`).");

      iterator nc_pos = begin () + (pos - cbegin ());
      if (pos + 1 != end ())
        std::move (nc_pos + 1, end (), nc_pos);
      pop_back ();
      return nc_pos;
    }

    GCH_CPP20_CONSTEXPR
    iterator
    erase (const_iterator first, const_iterator last)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::MoveAssignable<value_type>
#endif
    {
      assert (first < last  && "Invalid range.");

      assert (begin () <= first  && "The argument `first` is out of bounds (before `begin ()`)."  );
      assert (last     <  end () && "The argument `last` is out of bounds (at or after `end ()`).");

      iterator nc_first = begin () + (first - cbegin ());
      iterator nc_last  = begin () + (last  - cbegin ());

      if (first == last)
        return nc_last;

      iterator new_end = std::move (nc_last, end (), nc_first);
      base::destroy_range (new_end, end ());
      base::set_size (new_end - begin ());
      return nc_first;
    }

    GCH_CPP20_CONSTEXPR
    void
    push_back (const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::CopyInsertable<value_type, small_vector, allocator_type>
#endif
    {
      assert (! base::is_element (std::addressof (value)) &&
                "The argument `value` is an element of `*this`.");
      emplace_back (value);
    }

    GCH_CPP20_CONSTEXPR
    void
    push_back (value_type&& value)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::MoveInsertable<value_type, small_vector, allocator_type>
#endif
    {
      assert (! base::is_element (std::addressof (value)) &&
                "The argument `value` is an element of `*this`.");
      emplace_back (std::move (value));
    }

    template <typename ...Args>
    GCH_CPP20_CONSTEXPR
    reference
    emplace_back (Args&&... args)
#ifdef GCH_LIB_CONCEPTS
    requires concepts::MoveInsertable<value_type, small_vector, allocator_type> &&
             concepts::EmplaceConstructible<value_type, small_vector, allocator_type, Args...>
#endif
    {
      base::construct (base::request_elements_end (1), std::forward<Args> (args)...);
      base::set_size (size () + 1);
      return back ();
    }

    GCH_CPP20_CONSTEXPR
    void
    pop_back (void)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::Erasable<value_type, small_vector, allocator_type>
#endif
    {
      assert (! empty () && "`pop_back ()` called on an empty `small_vector`.");
      base::destroy (std::prev (end ()));
      base::set_size (size () - 1);
    }

    /* global state modification */
    GCH_CPP20_CONSTEXPR
    void
    reserve (size_type new_cap)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::MoveInsertable<value_type, small_vector, allocator_type>
#endif
    {
      base::request_capacity (new_cap);
    }

    GCH_CPP20_CONSTEXPR
    void
    shrink_to_fit (void)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::MoveInsertable<value_type, small_vector, allocator_type>
#endif
    {
      base::do_shrink_to_fit ();
    }

    GCH_CPP20_CONSTEXPR
    void
    clear (void) noexcept
#ifdef GCH_LIB_CONCEPTS
      requires concepts::Erasable<value_type, small_vector, allocator_type>
#endif
    {
      base::destroy_range (begin (), end ());
      base::set_size (0);
    }

    GCH_CPP20_CONSTEXPR
    void
    resize (size_type count)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::DefaultInsertable<value_type, small_vector, allocator_type> &&
               concepts::MoveInsertable<value_type, small_vector, allocator_type>
#endif
    {
      if (count == 0)
        clear ();

      if (size () < count)
      {
        base::request_elements_end (count - size ());
        base::uninitialized_value_construct (end (), begin () + count);
        base::set_size (count);
      }
      else if (count < size ())
        erase (std::next (cbegin (), count), cend ());
    }

    GCH_CPP20_CONSTEXPR
    void
    resize (size_type count, const_reference value)
#ifdef GCH_LIB_CONCEPTS
      requires concepts::CopyInsertable<value_type, small_vector, allocator_type>
#endif
    {
      if (count == 0)
        clear ();

      if (size () < count)
        insert (cend (), count - size (), value);
      else if (count < size ())
        erase (std::next (cbegin (), count), cend ());
    }

    /* non-standard */

    GCH_NODISCARD constexpr
    bool
    is_inline (void) const noexcept
    {
      return base::using_inline_storage ();
    }

    GCH_NODISCARD constexpr
    size_type
    inline_capacity (void) const noexcept
    {
      return InlineCapacity;
    }
  };


  /* support */
  namespace detail
  {

    template <typename Allocator, unsigned InlineCapacity>
    constexpr
    auto small_vector_base<Allocator, InlineCapacity>::as_storage (small_vector_base& self)
            noexcept
         -> inline_storage<value_t, InlineCapacity>&
    {
      return static_cast<inline_storage_t&> (static_cast<small_vector_t&> (self));
    }

    template <typename Allocator, unsigned InlineCapacity>
    constexpr
    auto small_vector_base<Allocator, InlineCapacity>::as_storage (const small_vector_base& self)
            noexcept
         -> const inline_storage<value_t, InlineCapacity>&
    {
      return static_cast<const inline_storage_t&> (static_cast<const small_vector_t&> (self));
    }

  } // namespace detail

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

  template <typename T, unsigned InlineCapacity, typename Allocator>
  GCH_CPP20_CONSTEXPR
  typename std::enable_if<small_vector_adl::is_swappable<T>::value &&
                          std::is_move_constructible<T>::value>::type
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
    typename small_vector<T, InlineCapacity, Allocator>::size_type initial_size = c.size ();
    c.erase (std::remove (c.begin (), c.end (), value), c.end ());
    return initial_size - c.size ();
  }

  template <typename T, unsigned InlineCapacity, typename Allocator, typename Pred>
  GCH_CPP20_CONSTEXPR
  typename small_vector<T, InlineCapacity, Allocator>::size_type
  erase_if (small_vector<T, InlineCapacity, Allocator>& c, Pred pred)
  {
    typename small_vector<T, InlineCapacity, Allocator>::size_type initial_size = c.size ();
    c.erase (std::remove_if (c.begin (), c.end (), pred), c.end ());
    return initial_size - c.size ();
  }

#ifdef GCH_CTAD_SUPPORT

  template <typename InputIt,
            unsigned InlineCapacity =
              default_buffer_size<typename std::iterator_traits<InputIt>::value_type>::value,
            typename Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>>
  small_vector (InputIt, InputIt, Allocator = Allocator ())
    -> small_vector<typename std::iterator_traits<InputIt>::value_type, InlineCapacity, Allocator>;

#endif

} // namespace gch

#endif // TESTBENCH_CPP_SMALL_VECTOR_HPP
