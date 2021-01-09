/** test_types.hpp.h
 * Short description here.
 *
 * Copyright © 2021 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GCH_SMALL_VECTOR_TEST_TEST_TYPES_HPP
#define GCH_SMALL_VECTOR_TEST_TEST_TYPES_HPP

#include <iostream>
#include <memory>

#include "gch/small_vector.hpp"


namespace gch
{
  namespace test_types
  {

    template <typename T>
    struct pointer_wrapper
    {
      using difference_type = typename std::iterator_traits<T*>::difference_type;
      using value_type = typename std::iterator_traits<T*>::value_type;
      using pointer = typename std::iterator_traits<T*>::pointer;
      using reference = typename std::iterator_traits<T*>::reference;
      using iterator_category = typename std::iterator_traits<T*>::iterator_category;
#ifdef GCH_LIB_CONCEPTS
      using iterator_concept = typename std::iterator_traits<T*>::iterator_concept;
#endif

      pointer_wrapper (void) = default;
      pointer_wrapper (const pointer_wrapper&) = default;
      pointer_wrapper (pointer_wrapper&&) noexcept = default;
      pointer_wrapper& operator= (const pointer_wrapper&) = default;
      pointer_wrapper& operator= (pointer_wrapper&&) noexcept = default;
      ~pointer_wrapper (void) = default;

      constexpr
      pointer_wrapper (T* p) noexcept
        : m_ptr (static_cast<T*> (p)) { }

      template <typename U,
        typename std::enable_if<std::is_convertible<U*, T*>::value>::type* = nullptr>
      constexpr
      pointer_wrapper (const pointer_wrapper<U>& other) noexcept
        : m_ptr (other.base ()) { }

      constexpr /* implicit */ pointer_wrapper (std::nullptr_t) noexcept
        : m_ptr (nullptr) { }

      template <typename U = T,
        typename std::enable_if<std::is_const<U>::value, bool>::type = true>
      constexpr /* implicit */ pointer_wrapper (const void* p) noexcept
        : m_ptr (static_cast<T*> (p)) { }

      constexpr /* implicit */ pointer_wrapper (void* p) noexcept
        : m_ptr (static_cast<T*> (p)) { }

      operator void* (void)
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
      reference operator* (void) const noexcept
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
      constexpr T* base (void) const noexcept
      {
        return m_ptr;
      }

    private:
      T* m_ptr;
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

    static constexpr T* to_address (pointer p) noexcept
    {
      return p.base ();
    }
  };
}

namespace gch
{
  namespace test_types
  {

    template <typename T>
    struct weird_allocator
      : public std::allocator<T>
    {
      using pointer = pointer_wrapper<T>;
      using const_pointer = pointer_wrapper<const T>;
      using void_pointer = void*;
      using const_void_pointer = const void*;

      template <typename ...Args>
      pointer allocate (Args&& ... args)
      {
        return std::allocator<T>::allocate (std::forward<Args> (args)...);
      }

      template <typename ...Args>
      void deallocate (pointer p, Args&& ... args)
      {
        std::allocator<T>::deallocate (p.operator-> (), std::forward<Args> (args)...);
      }
    };

    struct trivial
    {
      int data;
    };

    static_assert (std::is_trivial<trivial>::value, "not trivial");

    using trivial_array = trivial[5];

    static_assert (std::is_trivial<trivial_array>::value, "not trivial");

    struct trivially_copyable
    {
      trivially_copyable (void) = delete;
      trivially_copyable (const trivially_copyable&) = default;
      trivially_copyable (trivially_copyable&&) noexcept = default;
      trivially_copyable& operator= (const trivially_copyable&) = default;
      trivially_copyable& operator= (trivially_copyable&&) noexcept = default;
      ~trivially_copyable (void) = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable>::value,
                   "not trivially copyable");

    using trivially_copyable_array = trivially_copyable[5];
    static_assert (std::is_trivially_copyable<trivially_copyable_array>::value,
                   "not trivially copyable");

    struct trivially_copyable_copy_ctor
    {
      trivially_copyable_copy_ctor (void) = delete;
      trivially_copyable_copy_ctor (const trivially_copyable_copy_ctor&) = default;
      trivially_copyable_copy_ctor (trivially_copyable_copy_ctor&&) noexcept = delete;
      trivially_copyable_copy_ctor& operator= (const trivially_copyable_copy_ctor&) = delete;
      trivially_copyable_copy_ctor& operator= (trivially_copyable_copy_ctor&&) noexcept = delete;
      ~trivially_copyable_copy_ctor (void) = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_copy_ctor>::value,
                   "not trivially copyable");

    struct trivially_copyable_move_ctor
    {
      trivially_copyable_move_ctor (void) = delete;
      trivially_copyable_move_ctor (const trivially_copyable_move_ctor&) = delete;
      trivially_copyable_move_ctor (trivially_copyable_move_ctor&&) noexcept = default;
      trivially_copyable_move_ctor& operator= (const trivially_copyable_move_ctor&) = delete;
      trivially_copyable_move_ctor& operator= (trivially_copyable_move_ctor&&) noexcept = delete;
      ~trivially_copyable_move_ctor (void) = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_move_ctor>::value,
                   "not trivially copyable");

    struct trivially_copyable_copy_assign
    {
      trivially_copyable_copy_assign (void) = delete;
      trivially_copyable_copy_assign (const trivially_copyable_copy_assign&) = delete;
      trivially_copyable_copy_assign (trivially_copyable_copy_assign&&) noexcept = delete;
      trivially_copyable_copy_assign& operator= (const trivially_copyable_copy_assign&) = default;
      trivially_copyable_copy_assign&
      operator= (trivially_copyable_copy_assign&&) noexcept = delete;
      ~trivially_copyable_copy_assign (void) = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_copy_assign>::value,
                   "not trivially copyable");

    struct trivially_copyable_move_assign
    {
      trivially_copyable_move_assign (void) = delete;
      trivially_copyable_move_assign (const trivially_copyable_move_assign&) = delete;
      trivially_copyable_move_assign (trivially_copyable_move_assign&&) noexcept = delete;
      trivially_copyable_move_assign& operator= (const trivially_copyable_move_assign&) = delete;
      trivially_copyable_move_assign&
      operator= (trivially_copyable_move_assign&&) noexcept = default;
      ~trivially_copyable_move_assign (void) = default;
      int data;
    };

    static_assert (std::is_trivially_copyable<trivially_copyable_move_assign>::value,
                   "not trivially copyable");

    struct uncopyable
    {
      uncopyable (void) = default;
      uncopyable (const uncopyable&) = delete;
      uncopyable (uncopyable&&) noexcept = delete;
      uncopyable& operator= (const uncopyable&) = delete;
      uncopyable& operator= (uncopyable&&) noexcept = delete;
      ~uncopyable (void) = default;

      uncopyable (int x)
        : data (x) { }

      int data;
    };

    class non_trivial
    {
    public:
      constexpr
      non_trivial (void) noexcept
        : data (7) { }

      constexpr
      non_trivial (int x) noexcept
        : data (x + 3) { }

      constexpr
      non_trivial (const non_trivial& other) noexcept
        : data (other.data + 4) { }

      GCH_CPP14_CONSTEXPR
      non_trivial& operator= (const non_trivial& other) noexcept
      {
        if (&other != this)
          data = other.data + 5;
        return *this;
      }

      constexpr
      operator int (void) const noexcept
      {
        return data;
      }

      friend std::ostream& operator<< (std::ostream& o, const non_trivial& x)
      {
        return o << x.data;
      }

    private:
      int data;
    };
  }
}

#endif // GCH_SMALL_VECTOR_TEST_TEST_TYPES_HPP
