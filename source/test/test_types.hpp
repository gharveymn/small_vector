/** test_types.hpp
 * Short description here.
 *
 * Copyright © 2021 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GCH_SMALL_VECTOR_TEST_TYPES_HPP
#define GCH_SMALL_VECTOR_TEST_TYPES_HPP

#include "test_common.hpp"
#include "gch/small_vector.hpp"

#include <iostream>
#include <memory>
#include <stack>

namespace gch
{
  namespace test_types
  {

    template <typename T>
    struct pointer_wrapper
    {
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = T *;
      using reference = T&;
      using iterator_category = std::random_access_iterator_tag;
#ifdef GCH_LIB_CONCEPTS
      using iterator_concept = std::contiguous_iterator_tag;
#endif

      pointer_wrapper            (void)                       = default;
      pointer_wrapper            (const pointer_wrapper&)     = default;
      pointer_wrapper            (pointer_wrapper&&) noexcept = default;
      pointer_wrapper& operator= (const pointer_wrapper&)     = default;
      pointer_wrapper& operator= (pointer_wrapper&&) noexcept = default;
      ~pointer_wrapper           (void)                       = default;

      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (pointer p) noexcept
        : m_ptr (p)
      { }

      template <typename U,
        typename std::enable_if<std::is_convertible<U *, pointer>::value>::type* = nullptr>
      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (const pointer_wrapper<U>& other) noexcept
        : m_ptr (other.base ()) { }

      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (std::nullptr_t) noexcept
        : m_ptr (nullptr) { }

      template <typename U = T,
        typename std::enable_if<std::is_const<U>::value, bool>::type = true>
      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (const void *p) noexcept
        : m_ptr (static_cast<pointer> (p)) { }

      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (void *p) noexcept
        : m_ptr (static_cast<pointer> (p)) { }

      GCH_IMPLICIT_CONVERSION
      operator typename std::conditional<std::is_const<T>::value,
                                         const T *,
                                         pointer>::type (void)
      {
        return m_ptr;
      }

      GCH_CPP14_CONSTEXPR
      pointer_wrapper&
      operator++ (void) noexcept
      {
        ++m_ptr;
        return *this;
      }

      GCH_CPP14_CONSTEXPR
      pointer_wrapper
      operator++ (int) noexcept
      {
        return pointer_wrapper (m_ptr++);
      }

      GCH_CPP14_CONSTEXPR
      pointer_wrapper&
      operator-- (void) noexcept
      {
        --m_ptr;
        return *this;
      }

      GCH_CPP14_CONSTEXPR
      pointer_wrapper
      operator-- (int) noexcept
      {
        return pointer_wrapper (m_ptr--);
      }

      GCH_CPP14_CONSTEXPR
      pointer_wrapper&
      operator+= (difference_type n) noexcept
      {
        m_ptr += n;
        return *this;
      }

      GCH_NODISCARD
      constexpr
      pointer_wrapper
      operator+ (difference_type n) const noexcept
      {
        return pointer_wrapper (m_ptr + n);
      }

      GCH_CPP14_CONSTEXPR
      pointer_wrapper&
      operator-= (difference_type n) noexcept
      {
        m_ptr -= n;
        return *this;
      }

      GCH_NODISCARD
      constexpr
      pointer_wrapper
      operator- (difference_type n) const noexcept
      {
        return pointer_wrapper (m_ptr - n);
      }

      GCH_NODISCARD
      constexpr
      reference
      operator* (void) const noexcept
      {
        return *m_ptr;
      }

      GCH_NODISCARD
      constexpr
      pointer
      operator-> (void) const noexcept
      {
        return m_ptr;
      }

      GCH_NODISCARD
      constexpr
      reference
      operator[] (difference_type n) const noexcept
      {
        return m_ptr[n];
      }

      GCH_NODISCARD
      constexpr
      pointer
      base (void) const noexcept
      {
        return m_ptr;
      }

    private:
      pointer m_ptr;
    };

    template <>
    struct pointer_wrapper<void>
    {
      using pointer = void *;

      pointer_wrapper            (void)                       = default;
      pointer_wrapper            (const pointer_wrapper&)     = default;
      pointer_wrapper            (pointer_wrapper&&) noexcept = default;
      pointer_wrapper& operator= (const pointer_wrapper&)     = default;
      pointer_wrapper& operator= (pointer_wrapper&&) noexcept = default;
      ~pointer_wrapper           (void)                       = default;

      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (pointer p) noexcept
        : m_ptr (p)
      { }

      template <typename U,
        typename std::enable_if<std::is_convertible<U *, pointer>::value>::type* = nullptr>
      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (const pointer_wrapper<U>& other) noexcept
        : m_ptr (other.base ())
      { }

      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (std::nullptr_t) noexcept
        : m_ptr (nullptr)
      { }

      GCH_IMPLICIT_CONVERSION
      operator pointer (void)
      {
        return m_ptr;
      }

      GCH_NODISCARD
      constexpr
      pointer
      operator-> (void) const noexcept
      {
        return m_ptr;
      }

      GCH_NODISCARD
      constexpr
      pointer
      base (void) const noexcept
      {
        return m_ptr;
      }

    private:
      pointer m_ptr;
    };

    template <>
    struct pointer_wrapper<const void>
    {
      using pointer = const void *;

      pointer_wrapper            (void)                       = default;
      pointer_wrapper            (const pointer_wrapper&)     = default;
      pointer_wrapper            (pointer_wrapper&&) noexcept = default;
      pointer_wrapper& operator= (const pointer_wrapper&)     = default;
      pointer_wrapper& operator= (pointer_wrapper&&) noexcept = default;
      ~pointer_wrapper           (void)                       = default;

      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (pointer p) noexcept
        : m_ptr (p)
      { }

      template <typename U,
        typename std::enable_if<std::is_convertible<U *, pointer>::value>::type* = nullptr>
      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (const pointer_wrapper<U>& other) noexcept
        : m_ptr (other.base ())
      { }

      constexpr GCH_IMPLICIT_CONVERSION
      pointer_wrapper (std::nullptr_t) noexcept
        : m_ptr (nullptr)
      { }

      GCH_IMPLICIT_CONVERSION
      operator pointer (void)
      {
        return m_ptr;
      }

      GCH_NODISCARD
      constexpr
      pointer
      operator-> (void) const noexcept
      {
        return m_ptr;
      }

      GCH_NODISCARD
      constexpr
      pointer
      base (void) const noexcept
      {
        return m_ptr;
      }

    private:
      pointer m_ptr;
    };

#ifdef GCH_LIB_THREE_WAY_COMPARISON

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    bool
    operator== (const pointer_wrapper<PointerLHS>& lhs,
                const pointer_wrapper<PointerRHS>& rhs)
    noexcept (noexcept (lhs.base () == rhs.base ()))
    requires requires { { lhs.base () == rhs.base () } -> std::convertible_to<bool>; }
    {
      return lhs.base () == rhs.base ();
    }

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    auto
    operator<=> (const pointer_wrapper<PointerLHS>& lhs,
                 const pointer_wrapper<PointerRHS>& rhs)
    noexcept (noexcept (lhs.base () <=> rhs.base ()))
    requires std::three_way_comparable_with<PointerLHS, PointerRHS>
    {
      return lhs.base () <=> rhs.base ();
    }

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    auto
    operator<=> (const pointer_wrapper<PointerLHS>& lhs,
                 const pointer_wrapper<PointerRHS>& rhs)
    noexcept (noexcept (lhs.base () < rhs.base ()) && noexcept (rhs.base () < lhs.base ()))
    requires (!std::three_way_comparable_with<PointerLHS, PointerRHS>)
    {
      return (lhs.base () < rhs.base ()) ? std::weak_ordering::less
                                         : (rhs.base () < lhs.base ()) ? std::weak_ordering::greater
                                                                       : std::weak_ordering::equivalent;
    }

#else

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    bool
    operator== (const pointer_wrapper<PointerLHS>& lhs,
                const pointer_wrapper<PointerRHS>& rhs) noexcept
    {
      return lhs.base () == rhs.base ();
    }

    template <typename Pointer>
    constexpr
    bool
    operator== (const pointer_wrapper<Pointer>& lhs,
                const pointer_wrapper<Pointer>& rhs) noexcept
    {
      return lhs.base () == rhs.base ();
    }

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    bool
    operator!= (const pointer_wrapper<PointerLHS>& lhs,
                const pointer_wrapper<PointerRHS>& rhs) noexcept
    {
      return lhs.base () != rhs.base ();
    }

    template <typename Pointer>
    constexpr
    bool
    operator!= (const pointer_wrapper<Pointer>& lhs,
                const pointer_wrapper<Pointer>& rhs) noexcept
    {
      return lhs.base () != rhs.base ();
    }

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    bool
    operator< (const pointer_wrapper<PointerLHS>& lhs,
               const pointer_wrapper<PointerRHS>& rhs) noexcept
    {
      return lhs.base () < rhs.base ();
    }

    template <typename Pointer>
    constexpr
    bool
    operator< (const pointer_wrapper<Pointer>& lhs,
               const pointer_wrapper<Pointer>& rhs) noexcept
    {
      return lhs.base () < rhs.base ();
    }

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    bool
    operator> (const pointer_wrapper<PointerLHS>& lhs,
               const pointer_wrapper<PointerRHS>& rhs) noexcept
    {
      return lhs.base () > rhs.base ();
    }

    template <typename Pointer>
    constexpr
    bool
    operator> (const pointer_wrapper<Pointer>& lhs,
               const pointer_wrapper<Pointer>& rhs) noexcept
    {
      return lhs.base () > rhs.base ();
    }

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    bool
    operator<= (const pointer_wrapper<PointerLHS>& lhs,
                const pointer_wrapper<PointerRHS>& rhs) noexcept
    {
      return lhs.base () <= rhs.base ();
    }

    template <typename Pointer>
    constexpr
    bool
    operator<= (const pointer_wrapper<Pointer>& lhs,
                const pointer_wrapper<Pointer>& rhs) noexcept
    {
      return lhs.base () <= rhs.base ();
    }

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    bool
    operator>= (const pointer_wrapper<PointerLHS>& lhs,
                const pointer_wrapper<PointerRHS>& rhs) noexcept
    {
      return lhs.base () >= rhs.base ();
    }

    template <typename Pointer>
    constexpr
    bool
    operator>= (const pointer_wrapper<Pointer>& lhs,
                const pointer_wrapper<Pointer>& rhs) noexcept
    {
      return lhs.base () >= rhs.base ();
    }

#endif

    template <typename PointerLHS, typename PointerRHS>
    constexpr
    auto
    operator- (const pointer_wrapper<PointerLHS>& lhs,
               const pointer_wrapper<PointerRHS>& rhs) noexcept
    -> decltype (lhs.base () - rhs.base ())
    {
      return lhs.base () - rhs.base ();
    }

    template <typename Pointer>
    constexpr
    auto
    operator- (const pointer_wrapper<Pointer>& lhs,
               const pointer_wrapper<Pointer>& rhs) noexcept
    -> decltype (lhs.base () - rhs.base ())
    {
      return lhs.base () - rhs.base ();
    }

    template <typename Pointer>
    constexpr
    pointer_wrapper<Pointer>
    operator+ (typename pointer_wrapper<Pointer>::difference_type n,
               const pointer_wrapper<Pointer>& it) noexcept
    {
      return it + n;
    }

    template <typename T>
    GCH_NODISCARD
    constexpr
    bool
    operator== (std::nullptr_t, const pointer_wrapper<T>& rhs) noexcept
    {
      return nullptr == rhs.base ();
    }

    template <typename T>
    GCH_NODISCARD
    constexpr
    bool
    operator!= (std::nullptr_t, const pointer_wrapper<T>& rhs) noexcept
    {
      return nullptr != rhs.base ();
    }

    template <typename T>
    GCH_NODISCARD
    constexpr
    bool
    operator== (const pointer_wrapper<T>& lhs, std::nullptr_t) noexcept
    {
      return lhs.base () == nullptr;
    }

    template <typename T>
    GCH_NODISCARD
    constexpr
    bool
    operator!= (const pointer_wrapper<T>& lhs, std::nullptr_t) noexcept
    {
      return lhs.base () != nullptr;
    }
  }
}

namespace std
{

  template <typename T>
  struct pointer_traits<gch::test_types::pointer_wrapper<T>>
  {
    using pointer = gch::test_types::pointer_wrapper<T>;
    using element_type = T;
    using difference_type = typename pointer::difference_type;

    template <typename U>
    using rebind = gch::test_types::pointer_wrapper<U>;

    static constexpr pointer pointer_to (element_type& r) noexcept
    {
      return pointer (std::addressof (r));
    }

    static constexpr
    T *
    to_address (pointer p) noexcept
    {
      return p.base ();
    }
  };

}

namespace gch
{

  template <typename T, typename Allocator>
  using small_vector_with_allocator =
    small_vector<T, default_buffer_size<Allocator>::value, Allocator>;

  template <typename T, template <typename> class AllocatorT>
  using small_vector_with_allocator_template =
  small_vector<T, default_buffer_size<AllocatorT<T>>::value, AllocatorT<T>>;

  namespace test_types
  {

    struct trivially_copyable_data_base
    {

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
      constexpr
      trivially_copyable_data_base (void) noexcept
        : data { }
      { }
#else
      trivially_copyable_data_base (void) = default;
#endif

      constexpr
      trivially_copyable_data_base (int x) noexcept
        : data (x)
      { }

      int data;
    };

    constexpr
    bool
    operator== (const trivially_copyable_data_base& lhs, const trivially_copyable_data_base& rhs)
    {
      return lhs.data == rhs.data;
    }

    constexpr
    bool
    operator!= (const trivially_copyable_data_base& lhs, const trivially_copyable_data_base& rhs)
    {
      return ! (lhs == rhs);
    }

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
    static_assert (
      ! std::is_trivially_default_constructible<trivially_copyable_data_base>::value,
      "Unexpectedly trivially default constructible.");
#else
    static_assert (
      std::is_trivial<trivially_copyable_data_base>::value,
      "Unexpectedly not trivial.");

    static_assert (
      std::is_trivially_default_constructible<trivially_copyable_data_base>::value,
      "Unexpectedly not trivially default constructible.");
#endif

    static_assert (
      std::is_trivially_copyable<trivially_copyable_data_base>::value,
      "Unexpectedly not trivially copyable.");

    struct nontrivial_data_base
      : trivially_copyable_data_base
    {
      using trivially_copyable_data_base::trivially_copyable_data_base;

      constexpr
      nontrivial_data_base (void) noexcept
        : trivially_copyable_data_base ()
      { }

      constexpr
      nontrivial_data_base (const nontrivial_data_base& other) noexcept
        : trivially_copyable_data_base (other)
      { }

      nontrivial_data_base& operator= (const nontrivial_data_base& other) = default;

      GCH_CPP20_CONSTEXPR
      ~nontrivial_data_base (void) noexcept
      {
        // Overwrite data to catch uninitialized errors.
        data = 0x7AFE;
      }
    };

    static_assert (
      ! std::is_trivially_default_constructible<nontrivial_data_base>::value ,
      "Unexpectedly trivially default constructible.");

    static_assert (
      ! std::is_trivially_copyable<nontrivial_data_base>::value,
      "Unexpectedly trivially copyable.");

#ifdef GCH_EXCEPTIONS

    struct test_exception
      : std::exception
    {
      const char *
      what (void) const noexcept override
      {
        return "test exception";
      }
    };

    class global_exception_trigger_tracker
    {
    public:

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
      constexpr
      void
      operator() (void) const noexcept
      { }

      constexpr
      void
      push (std::size_t) const noexcept
      { }

      constexpr
      void
      reset (void) const noexcept
      { }

#else

      void
      operator() (void)
      {
        if (m_stack.empty ())
          return;

        if (0 == m_stack.top ()--)
        {
          m_stack.pop ();
          throw test_exception { };
        }
      }

      void
      push (std::size_t n)
      {
        m_stack.push (n);
      }

      void
      reset (void) noexcept
      {
        m_stack = { };
      }

    private:
      std::stack<std::size_t> m_stack;
#endif
    };

    extern global_exception_trigger_tracker global_exception_trigger;

    struct triggering_copy_ctor
      : trivially_copyable_data_base
    {
#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
      triggering_copy_ctor (const triggering_copy_ctor&) = default;
#else
      triggering_copy_ctor (const triggering_copy_ctor& other)
        : trivially_copyable_data_base (other)
      {
        global_exception_trigger ();
      }
#endif

      triggering_copy_ctor& operator= (const triggering_copy_ctor&) = default;

      using trivially_copyable_data_base::trivially_copyable_data_base;
    };

    struct triggering_move_ctor
      : trivially_copyable_data_base
    {
      triggering_move_ctor            (void)                            = default;
      triggering_move_ctor            (const triggering_move_ctor&)     = default;
//    triggering_move_ctor            (triggering_move_ctor&&) noexcept = impl;
      triggering_move_ctor& operator= (const triggering_move_ctor&)     = default;
      triggering_move_ctor& operator= (triggering_move_ctor&&) noexcept = default;
      ~triggering_move_ctor           (void)                            = default;

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
      triggering_move_ctor (triggering_move_ctor&&) noexcept = default;
#else
      triggering_move_ctor (triggering_move_ctor&& other) noexcept (false)
        : trivially_copyable_data_base (std::move (other))
      {
        global_exception_trigger ();
      }
#endif

      using trivially_copyable_data_base::trivially_copyable_data_base;
    };

    struct triggering_move
      : triggering_move_ctor
    {
      triggering_move            (void)                       = default;
      triggering_move            (const triggering_move&)     = default;
      triggering_move            (triggering_move&&)          = default;
      triggering_move& operator= (const triggering_move&)     = default;
//    triggering_move& operator= (triggering_move&&) noexcept = impl;
      ~triggering_move           (void)                       = default;

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
      triggering_move& operator= (triggering_move&&) noexcept = default;
#else
      triggering_move& operator= (triggering_move&& other)
      {
        triggering_move_ctor::operator= (other);
        global_exception_trigger ();
        return *this;
      }
#endif

      using triggering_move_ctor::triggering_move_ctor;
    };


    struct triggering_ctor
      : trivially_copyable_data_base
    {
      triggering_ctor& operator= (const triggering_ctor&)     = default;
      triggering_ctor& operator= (triggering_ctor&&) noexcept = default;

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
      triggering_ctor (const triggering_ctor&) = default;
      triggering_ctor (triggering_ctor&&) noexcept = default;
#else
      triggering_ctor (const triggering_ctor& other)
        : trivially_copyable_data_base (other)
      {
        global_exception_trigger ();
      }

      triggering_ctor (triggering_ctor&& other) noexcept (false)
        : trivially_copyable_data_base (std::move (other))
      {
        // Some irrelevant code to quiet compiler warnings.
        delete new int;
      }
#endif

      using trivially_copyable_data_base::trivially_copyable_data_base;
    };

#endif

    struct trivial
    {
      int data;
    };

    static_assert (std::is_trivial<trivial>::value, "not trivial");

    using trivial_array = trivial[5];

    static_assert (std::is_trivial<trivial_array>::value, "not trivial");

    struct trivially_copyable
    {
      trivially_copyable            (void)                          = delete;
      trivially_copyable            (const trivially_copyable&)     = default;
      trivially_copyable            (trivially_copyable&&) noexcept = default;
      trivially_copyable& operator= (const trivially_copyable&)     = default;
      trivially_copyable& operator= (trivially_copyable&&) noexcept = default;
      ~trivially_copyable           (void)                          = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable>::value,
                   "not trivially copyable");

    using trivially_copyable_array = trivially_copyable[5];
    static_assert (std::is_trivially_copyable<trivially_copyable_array>::value,
                   "not trivially copyable");

    struct trivially_copyable_copy_ctor
    {
      trivially_copyable_copy_ctor            (void)                                    = delete;
      trivially_copyable_copy_ctor            (const trivially_copyable_copy_ctor&)     = default;
      trivially_copyable_copy_ctor            (trivially_copyable_copy_ctor&&) noexcept = delete;
      trivially_copyable_copy_ctor& operator= (const trivially_copyable_copy_ctor&)     = delete;
      trivially_copyable_copy_ctor& operator= (trivially_copyable_copy_ctor&&) noexcept = delete;
      ~trivially_copyable_copy_ctor           (void)                                    = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_copy_ctor>::value,
                   "not trivially copyable");

    struct trivially_copyable_move_ctor
    {
      trivially_copyable_move_ctor            (void)                                    = delete;
      trivially_copyable_move_ctor            (const trivially_copyable_move_ctor&)     = delete;
      trivially_copyable_move_ctor            (trivially_copyable_move_ctor&&) noexcept = default;
      trivially_copyable_move_ctor& operator= (const trivially_copyable_move_ctor&)     = delete;
      trivially_copyable_move_ctor& operator= (trivially_copyable_move_ctor&&) noexcept = delete;
      ~trivially_copyable_move_ctor           (void)                                    = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_move_ctor>::value,
                   "not trivially copyable");

    struct trivially_copyable_copy_assign
    {
      trivially_copyable_copy_assign            (void)                                      = delete;
      trivially_copyable_copy_assign            (const trivially_copyable_copy_assign&)     = delete;
      trivially_copyable_copy_assign            (trivially_copyable_copy_assign&&) noexcept = delete;
      trivially_copyable_copy_assign& operator= (const trivially_copyable_copy_assign&)     = default;
      trivially_copyable_copy_assign& operator= (trivially_copyable_copy_assign&&) noexcept = delete;
      ~trivially_copyable_copy_assign           (void)                                      = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_copy_assign>::value,
                   "not trivially copyable");

    struct trivially_copyable_move_assign
    {
      trivially_copyable_move_assign            (void)                                      = delete;
      trivially_copyable_move_assign            (const trivially_copyable_move_assign&)     = delete;
      trivially_copyable_move_assign            (trivially_copyable_move_assign&&) noexcept = delete;
      trivially_copyable_move_assign& operator= (const trivially_copyable_move_assign&)     = delete;
      trivially_copyable_move_assign& operator= (trivially_copyable_move_assign&&) noexcept = default;
      ~trivially_copyable_move_assign           (void)                                      = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_move_assign>::value,
                   "not trivially copyable");

    struct uncopyable
    {
      uncopyable            (void)                  = default;
      uncopyable            (const uncopyable&)     = delete;
      uncopyable            (uncopyable&&) noexcept = delete;
      uncopyable& operator= (const uncopyable&)     = delete;
      uncopyable& operator= (uncopyable&&) noexcept = delete;
      ~uncopyable           (void)                  = default;

      uncopyable (int x)
        : data (x) { }

      int data;
    };

    class non_trivial
    {
    public:
      constexpr
      non_trivial (void) noexcept
        : data (7)
      { }

      constexpr
      non_trivial (int x) noexcept
        : data (x)
      { }

      constexpr
      non_trivial (const non_trivial& other) noexcept
        : data (other.data)
      { }

      GCH_CPP14_CONSTEXPR
      non_trivial&
      operator= (const non_trivial& other) noexcept
      {
#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
        if (std::is_constant_evaluated ())
        {
          data = other.data;
          return *this;
        }
#endif
        if (&other != this)
          data = other.data;
        return *this;
      }

      constexpr
      operator int (void) const noexcept
      {
        return data;
      }

      friend
      std::ostream&
      operator<< (std::ostream& o, const non_trivial& x)
      {
        return o << x.data;
      }

    private:
      int data;
    };

  }

}

#endif // GCH_SMALL_VECTOR_TEST_TYPES_HPP
