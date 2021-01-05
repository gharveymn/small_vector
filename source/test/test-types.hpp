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

namespace gch
{

  namespace test_types
  {

    template <typename T>
    struct pointer_wrapper1
      : std::iterator_traits<T *>
    {
      using pointer         = typename std::pointer_traits<T *>::pointer;
      using difference_type = typename std::pointer_traits<T *>::difference_type;

      pointer_wrapper1            (void)                        = default;
      pointer_wrapper1            (const pointer_wrapper1&)     = default;
      pointer_wrapper1            (pointer_wrapper1&&) noexcept = default;
      pointer_wrapper1& operator= (const pointer_wrapper1&)     = default;
      pointer_wrapper1& operator= (pointer_wrapper1&&) noexcept = default;
      ~pointer_wrapper1           (void)                        = default;

      template <typename U,
                typename = typename std::enable_if<std::is_constructible<T *, U *>::value>::type,
                typename std::enable_if<std::is_convertible<U *, T*>::value, bool>::type = true>
      constexpr /* implicit */ pointer_wrapper1 (U *p) noexcept
        : m_ptr (p)
      { }

      template <typename U,
                typename = typename std::enable_if<std::is_constructible<T *, U *>::value>::type,
                typename std::enable_if<! std::is_convertible<U *, T*>::value, bool>::type = false>
      constexpr /* implicit */ pointer_wrapper1 (U *p) noexcept
        : m_ptr (static_cast<T *> (p))
      { }

      template <typename U,
                typename = typename std::enable_if<std::is_constructible<T *, U *>::value>::type,
                typename std::enable_if<std::is_convertible<U *, T*>::value, bool>::type = true>
      constexpr /* implicit */ pointer_wrapper1 (const pointer_wrapper1<U>& other) noexcept
        : m_ptr (other.base ())
      { }

      constexpr /* implicit */ pointer_wrapper1 (std::nullptr_t) noexcept
        : m_ptr (nullptr)
      { }

      template <typename U = T,
                typename std::enable_if<std::is_const<U>::value, bool>::type = true>
      constexpr /* implicit */ pointer_wrapper1 (const void *p) noexcept
        : m_ptr (static_cast<T *> (p))
      { }

      constexpr /* implicit */ pointer_wrapper1 (void *p) noexcept
        : m_ptr (static_cast<T *> (p))
      { }

      operator void * (void)
      {
        return m_ptr;
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper1& operator++ (void) noexcept
      {
        ++m_ptr;
        return *this;
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper1 operator++ (int) noexcept
      {
        return pointer_wrapper1 (m_ptr++);
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper1& operator-- (void) noexcept
      {
        --m_ptr;
        return *this;
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper1 operator-- (int) noexcept
      {
        return pointer_wrapper1 (m_ptr--);
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper1& operator+= (difference_type n)
      {
        m_ptr += n;
        return *this;
      }

      GCH_NODISCARD
      constexpr pointer_wrapper1 operator+ (difference_type n) const
      {
        return pointer_wrapper1 (m_ptr + n);
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper1& operator-= (difference_type n)
      {
        m_ptr -= n;
        return *this;
      }

      GCH_NODISCARD
      constexpr pointer_wrapper1 operator- (difference_type n) const
      {
        return pointer_wrapper1 (m_ptr - n);
      }

      GCH_NODISCARD
      constexpr typename std::iterator_traits<T *>::reference operator[] (difference_type n) const
      {
        return operator+ (n).operator* ();
      }

      GCH_NODISCARD
      constexpr typename std::iterator_traits<T *>::reference operator* (void) const
      {
        return *m_ptr;
      }

      GCH_NODISCARD
      constexpr typename std::iterator_traits<T *>::pointer operator-> (void) const noexcept
      {
        return m_ptr;
      }

      GCH_NODISCARD
      constexpr T * base (void) const noexcept
      {
        return m_ptr;
      }

    private:
      T *m_ptr;
    };

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator< (const pointer_wrapper1<T>& lhs, const pointer_wrapper1<U>& rhs)
    noexcept (noexcept (lhs.base () < rhs.base ()))
    -> decltype (lhs.base () < rhs.base ())
    {
      return lhs.base () < rhs.base ();
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator> (const pointer_wrapper1<T>& lhs, const pointer_wrapper1<U>& rhs)
    noexcept (noexcept (lhs.base () > rhs.base ()))
    -> decltype (lhs.base () > rhs.base ())
    {
      return lhs.base () > rhs.base ();
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator<= (const pointer_wrapper1<T>& lhs, const pointer_wrapper1<U>& rhs)
    noexcept (noexcept (lhs.base () <= rhs.base ()))
    -> decltype (lhs.base () <= rhs.base ())
    {
      return lhs.base () <= rhs.base ();
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator>= (const pointer_wrapper1<T>& lhs, const pointer_wrapper1<U>& rhs)
    noexcept (noexcept (lhs.base () >= rhs.base ()))
    -> decltype (lhs.base () >= rhs.base ())
    {
      return lhs.base () >= rhs.base ();
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator== (const pointer_wrapper1<T>& lhs, const pointer_wrapper1<U>& rhs)
    noexcept (noexcept (lhs.base () == rhs.base ()))
    -> decltype (lhs.base () == rhs.base ())
    {
      return lhs.base () == rhs.base ();
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator!= (const pointer_wrapper1<T>& lhs, const pointer_wrapper1<U>& rhs)
    noexcept (noexcept (lhs.base () != rhs.base ()))
    -> decltype (lhs.base () != rhs.base ())
    {
      return lhs.base () != rhs.base ();
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator- (const pointer_wrapper1<T>& lhs, const pointer_wrapper1<U>& rhs)
    noexcept (noexcept (lhs.base () - rhs.base ()))
    -> decltype (lhs.base () - rhs.base ())
    {
      return lhs.base () - rhs.base ();
    }

    template <typename T>
    GCH_NODISCARD constexpr
    pointer_wrapper1<T>
    operator+ (typename pointer_wrapper1<T>::difference_type n, const pointer_wrapper1<T>& it)
    {
      return pointer_wrapper1<T> (n + it.base ());
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator== (std::nullptr_t, const pointer_wrapper1<T>& rhs)
    noexcept (noexcept (nullptr == rhs.base ()))
    -> decltype (nullptr == rhs.base ())
    {
      return nullptr == rhs.base ();
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator!= (std::nullptr_t, const pointer_wrapper1<T>& rhs)
    noexcept (noexcept (nullptr != rhs.base ()))
    -> decltype (nullptr != rhs.base ())
    {
      return nullptr != rhs.base ();
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator== (const pointer_wrapper1<T>& lhs, std::nullptr_t)
    noexcept (noexcept (lhs.base () == nullptr))
    -> decltype (lhs.base () == nullptr)
    {
      return lhs.base () == nullptr;
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator!= (const pointer_wrapper1<T>& lhs, std::nullptr_t)
    noexcept (noexcept (lhs.base () != nullptr))
    -> decltype (lhs.base () != nullptr)
    {
      return lhs.base () != nullptr;
    }

    template <typename T>
    struct pointer_wrapper2
      : std::iterator_traits<T *>
    {
      friend std::pointer_traits<pointer_wrapper2<T>>;

      using pointer         = typename std::pointer_traits<T *>::pointer;
      using difference_type = typename std::pointer_traits<T *>::difference_type;

      pointer_wrapper2            (void)                        = default;
      pointer_wrapper2            (const pointer_wrapper2&)     = default;
      pointer_wrapper2            (pointer_wrapper2&&) noexcept = default;
      pointer_wrapper2& operator= (const pointer_wrapper2&)     = default;
      pointer_wrapper2& operator= (pointer_wrapper2&&) noexcept = default;
      ~pointer_wrapper2           (void)                        = default;

      template <typename U,
                typename = typename std::enable_if<std::is_constructible<T *, U *>::value>::type,
        typename std::enable_if<std::is_convertible<U *, T*>::value, bool>::type = true>
      constexpr /* implicit */ pointer_wrapper2 (U *p) noexcept
        : m_ptr (p)
      { }

      template <typename U,
                typename = typename std::enable_if<std::is_constructible<T *, U *>::value>::type,
        typename std::enable_if<! std::is_convertible<U *, T*>::value, bool>::type = false>
      constexpr /* implicit */ pointer_wrapper2 (U *p) noexcept
      : m_ptr (static_cast<T *> (p))
      { }

      template <typename U,
                typename = typename std::enable_if<std::is_constructible<T *, U *>::value>::type,
        typename std::enable_if<std::is_convertible<U *, T*>::value, bool>::type = true>
      constexpr /* implicit */ pointer_wrapper2 (const pointer_wrapper2<U>& other) noexcept
        : m_ptr (other.base ())
      { }

      constexpr /* implicit */ pointer_wrapper2 (std::nullptr_t) noexcept
      : m_ptr (nullptr)
      { }

      template <typename U = T,
        typename std::enable_if<std::is_const<U>::value, bool>::type = true>
      constexpr /* implicit */ pointer_wrapper2 (const void *p) noexcept
        : m_ptr (static_cast<T *> (p))
      { }

      constexpr /* implicit */ pointer_wrapper2 (void *p) noexcept
        : m_ptr (static_cast<T *> (p))
      { }

      operator void * (void)
      {
        return m_ptr;
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper2& operator++ (void) noexcept
      {
        ++m_ptr;
        return *this;
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper2 operator++ (int) noexcept
      {
        return pointer_wrapper2 (m_ptr++);
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper2& operator-- (void) noexcept
      {
        --m_ptr;
        return *this;
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper2 operator-- (int) noexcept
      {
        return pointer_wrapper2 (m_ptr--);
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper2& operator+= (difference_type n)
      {
        m_ptr += n;
        return *this;
      }

      GCH_NODISCARD
      constexpr pointer_wrapper2 operator+ (difference_type n) const
      {
        return pointer_wrapper2 (m_ptr + n);
      }

      GCH_CPP14_CONSTEXPR pointer_wrapper2& operator-= (difference_type n)
      {
        m_ptr -= n;
        return *this;
      }

      GCH_NODISCARD
      constexpr pointer_wrapper2 operator- (difference_type n) const
      {
        return pointer_wrapper2 (m_ptr - n);
      }

      GCH_NODISCARD
      constexpr typename std::iterator_traits<T *>::reference operator[] (difference_type n) const
      {
        return operator+ (n).operator* ();
      }

      GCH_NODISCARD
      constexpr typename std::iterator_traits<T *>::reference operator* (void) const
      {
        return *m_ptr;
      }

      GCH_NODISCARD
      constexpr typename std::iterator_traits<T *>::pointer operator-> (void) const noexcept = delete;

      GCH_NODISCARD
      constexpr T * base (void) const noexcept
      {
        return m_ptr;
      }

    private:
      T *m_ptr;
    };

    template <typename T>
    constexpr T * to_address (const pointer_wrapper2<T>& p)
    {
      return std::pointer_traits<pointer_wrapper2<T>>::to_address (p);
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator< (const pointer_wrapper2<T>& lhs, const pointer_wrapper2<U>& rhs)
    noexcept (noexcept (to_address (lhs) < to_address (rhs)))
    -> decltype (to_address (lhs) < to_address (rhs))
    {
      return to_address (lhs) < to_address (rhs);
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator> (const pointer_wrapper2<T>& lhs, const pointer_wrapper2<U>& rhs)
    noexcept (noexcept (to_address (lhs) > to_address (rhs)))
    -> decltype (to_address (lhs) > to_address (rhs))
    {
      return to_address (lhs) > to_address (rhs);
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator<= (const pointer_wrapper2<T>& lhs, const pointer_wrapper2<U>& rhs)
    noexcept (noexcept (to_address (lhs) <= to_address (rhs)))
    -> decltype (to_address (lhs) <= to_address (rhs))
    {
      return to_address (lhs) <= to_address (rhs);
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator>= (const pointer_wrapper2<T>& lhs, const pointer_wrapper2<U>& rhs)
    noexcept (noexcept (to_address (lhs) >= to_address (rhs)))
    -> decltype (to_address (lhs) >= to_address (rhs))
    {
      return to_address (lhs) >= to_address (rhs);
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator== (const pointer_wrapper2<T>& lhs, const pointer_wrapper2<U>& rhs)
    noexcept (noexcept (to_address (lhs) == to_address (rhs)))
    -> decltype (to_address (lhs) == to_address (rhs))
    {
      return to_address (lhs) == to_address (rhs);
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator!= (const pointer_wrapper2<T>& lhs, const pointer_wrapper2<U>& rhs)
    noexcept (noexcept (to_address (lhs) != to_address (rhs)))
    -> decltype (to_address (lhs) != to_address (rhs))
    {
      return to_address (lhs) != to_address (rhs);
    }

    template <typename T, typename U>
    GCH_NODISCARD constexpr
    auto operator- (const pointer_wrapper2<T>& lhs, const pointer_wrapper2<U>& rhs)
    noexcept (noexcept (to_address (lhs) - to_address (rhs)))
    -> decltype (to_address (lhs) - to_address (rhs))
    {
      return to_address (lhs) - to_address (rhs);
    }

    template <typename T>
    GCH_NODISCARD constexpr
    pointer_wrapper2<T>
    operator+ (typename pointer_wrapper2<T>::difference_type n, const pointer_wrapper2<T>& it)
    {
      return pointer_wrapper2<T> (n + to_address (it));
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator== (std::nullptr_t, const pointer_wrapper2<T>& rhs)
    noexcept (noexcept (nullptr == rhs.base ()))
    -> decltype (nullptr == rhs.base ())
    {
      return nullptr == rhs.base ();
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator!= (std::nullptr_t, const pointer_wrapper2<T>& rhs)
    noexcept (noexcept (nullptr != rhs.base ()))
    -> decltype (nullptr != rhs.base ())
    {
      return nullptr != rhs.base ();
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator== (const pointer_wrapper2<T>& lhs, std::nullptr_t)
    noexcept (noexcept (lhs.base () == nullptr))
    -> decltype (lhs.base () == nullptr)
    {
      return lhs.base () == nullptr;
    }

    template <typename T>
    GCH_NODISCARD constexpr
    auto operator!= (const pointer_wrapper2<T>& lhs, std::nullptr_t)
    noexcept (noexcept (lhs.base () != nullptr))
    -> decltype (lhs.base () != nullptr)
    {
      return lhs.base () != nullptr;
    }

    template <typename T>
    struct weird_allocator1
      : public std::allocator<T>
    {
      using pointer       = pointer_wrapper1<T>;
      using const_pointer = pointer_wrapper1<const T>;
      using void_pointer = void *;
      using const_void_pointer = const void *;

      template <typename ...Args>
      pointer allocate (Args&&... args)
      {
        return std::allocator<T>::allocate(std::forward<Args> (args)...);
      }

      template <typename ...Args>
      void deallocate (Args&&... args)
      {
        std::allocator<T>::deallocate(std::forward<Args> (args)...);
      }
    };

    template <typename T>
    struct weird_allocator2
      : public std::allocator<T>
    {
      using pointer       = pointer_wrapper2<T>;
      using const_pointer = pointer_wrapper2<const T>;
      using void_pointer = void *;
      using const_void_pointer = const void *;

      template <typename ...Args>
      pointer allocate (Args&&... args)
      {
        return std::allocator<T>::allocate(std::forward<Args> (args)...);
      }

      template <typename ...Args>
      void deallocate (Args&&... args)
      {
        std::allocator<T>::deallocate(std::forward<Args> (args)...);
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

      uncopyable (int x) : data (x) { }
      int data;
    };

    class non_trivial
    {
    public:
      non_trivial (void)
        : data (7)
      { }

      non_trivial (int x)
        : data (x + 3)
      { }

      non_trivial (const non_trivial& other)
        : data (other.data + 4)
      { }

      non_trivial& operator= (const non_trivial& other)
      {
        if (&other != this)
          data = other.data + 5;
        return *this;
      }

      operator int (void)
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

namespace std
{

  template <typename T>
  struct pointer_traits<gch::test_types::pointer_wrapper2<T>>
  {
    using pointer         = gch::test_types::pointer_wrapper2<T>;
    using element_type    = T;
    using difference_type = typename pointer::difference_type;

    template <typename U>
    using rebind = gch::test_types::pointer_wrapper2<U>;

    static constexpr pointer pointer_to (element_type& r) noexcept
    {
      return pointer (std::addressof (r));
    }

    static constexpr T * to_address (pointer p) noexcept
    {
      return p.m_ptr;
    }
  };

}

#endif // GCH_SMALL_VECTOR_TEST_TEST_TYPES_HPP
