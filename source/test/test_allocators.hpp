/** test-allocators.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SMALL_VECTOR_TEST_ALLOCATORS_HPP
#define SMALL_VECTOR_TEST_ALLOCATORS_HPP

#include "test_common.hpp"

#include <memory>
#include <unordered_map>

namespace gch
{

  namespace test_types
  {

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

    template <typename T>
    constexpr
    T *
    to_address (T *p) noexcept
    {
      static_assert (! std::is_function<T>::value, "value_ty is a function pointer.");
      return p;
    }

    template <typename Pointer,
      typename std::enable_if<has_ptr_traits_to_address<Pointer>::value>::type * = nullptr>
    constexpr
    auto
    to_address (const Pointer& p) noexcept
    -> decltype (std::pointer_traits<Pointer>::to_address (p))
    {
      return std::pointer_traits<Pointer>::to_address (p);
    }

    template <typename Pointer,
      typename std::enable_if<! has_ptr_traits_to_address<Pointer>::value>::type * = nullptr>
    constexpr
    auto
    to_address (const Pointer& p) noexcept
    -> decltype (to_address (p.operator-> ()))
    {
      return to_address (p.operator-> ());
    }

    template <typename T>
    struct allocator_difference_type_trait
    {
      using difference_type = T;
    };

    template <typename T>
    struct allocator_size_type_trait
    {
      using size_type = T;
    };

    template <typename T>
    struct allocator_pointer_trait
    {
      using pointer = T;
    };

    template <typename T, typename PartialTraits = std::allocator_traits<std::allocator<T>>>
    class base_allocator
    {
      template <typename Traits, typename Enable = void>
      struct extracted_difference_type
      {
        using type = typename std::allocator_traits<std::allocator<T>>::difference_type;
      };

      template <typename Traits>
      struct extracted_difference_type<Traits, void_t<typename Traits::difference_type>>
      {
        using type = typename Traits::difference_type;
      };

      template <typename Traits, typename Enable = void>
      struct extracted_size_type
      {
        using type = typename std::allocator_traits<std::allocator<T>>::size_type;
      };

      template <typename Traits>
      struct extracted_size_type<Traits, void_t<typename Traits::size_type>>
      {
        using type = typename Traits::size_type;
      };

      template <typename Traits, typename Enable = void>
      struct extracted_pointer_type
      {
        using type = typename std::allocator_traits<std::allocator<T>>::pointer;
      };

      template <typename Traits>
      struct extracted_pointer_type<Traits, void_t<typename Traits::pointer>>
      {
        using type = typename Traits::pointer;
      };

    public:
      using value_type = T;
      using difference_type = typename extracted_difference_type<PartialTraits>::type;
      using size_type = typename extracted_size_type<PartialTraits>::type;
      using pointer = typename extracted_pointer_type<PartialTraits>::type;
      using propagate_on_container_move_assignment = std::true_type;
      using is_always_equal = std::true_type;

      base_allocator (void) = default;

      template <typename U, typename>
      constexpr
      base_allocator (const base_allocator<U, PartialTraits>&) noexcept
      { }

      GCH_CPP20_CONSTEXPR
      pointer
      allocate (size_type n)
      {
        return pointer (std::allocator<value_type> ().allocate (n));
      }

      GCH_CPP20_CONSTEXPR
      void
      deallocate (pointer p, size_type n) noexcept
      {
        std::allocator<value_type> ().deallocate (to_address (p), n);
      }
    };

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator== (const base_allocator<T, Traits>&, const base_allocator<U, Traits>&) noexcept
    {
      return true;
    }

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator!= (const base_allocator<T, Traits>&, const base_allocator<U, Traits>&) noexcept
    {
      return false;
    }

    template <typename T, typename PartialTraits = std::allocator_traits<std::allocator<T>>>
    class allocator_with_id
      : public base_allocator<T, PartialTraits>
    {
      using base = base_allocator<T, PartialTraits>;

    public:
      using is_always_equal = std::false_type;

      allocator_with_id (void) = default;

      constexpr explicit
      allocator_with_id (int id) noexcept
        : m_id (id)
      { }

      template <typename U>
      constexpr
      allocator_with_id (const allocator_with_id<U, PartialTraits>& other) noexcept
        : base (other),
          m_id (other.m_id)
      { }

      constexpr
      int
      get_id (void) const noexcept
      {
        return m_id;
      }

    private:
      int m_id = 0;
    };

    template <typename T, typename U, typename SizeType>
    constexpr
    bool
    operator== (const allocator_with_id<T, SizeType>& lhs,
                const allocator_with_id<U, SizeType>& rhs) noexcept
    {
      return lhs.get_id () == rhs.get_id ();
    }

    template <typename T, typename U, typename SizeType>
    constexpr
    bool
    operator!= (const allocator_with_id<T, SizeType>& lhs,
                const allocator_with_id<U, SizeType>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    class verifying_allocator_base
    {
      using alloc_map = std::unordered_map<int, std::unordered_map<void *, std::size_t>>;

      static alloc_map&
      get_map (void)
      {
         static alloc_map map;
         return map;
      }

    public:
      template <typename T, typename Pointer>
      static void
      register_allocation (const allocator_with_id<T>& alloc, Pointer p, std::size_t n)
      {
        get_map ()[alloc.get_id ()].emplace (static_cast<void *> (to_address (p)), n);
      }

      template <typename T, typename Pointer>
      static void
      check_allocation (const allocator_with_id<T>& alloc, Pointer p, std::size_t n)
      {
        auto alloctor_it = get_map ().find (alloc.get_id ());
        assert (alloctor_it != get_map ().end () && "The allocator has not been used yet.");

        auto allocation_it = alloctor_it->second.find (static_cast<void *> (to_address (p)));
        assert (allocation_it != alloctor_it->second.end ()
            &&  "Could not find the allocation for the given allocator.");

        assert (allocation_it->second == n && "The allocation size did not match the given size.");
      }

      template <typename T, typename Pointer>
      static void
      remove_allocation (const allocator_with_id<T>& alloc, Pointer p, std::size_t n)
      {
        check_allocation (alloc, p, n);
        auto& allocation_map = get_map ().find (alloc.get_id ())->second;
        allocation_map.erase (static_cast<void *> (to_address (p)));
        if (allocation_map.empty ())
          get_map ().erase (alloc.get_id ());
      }
    };

    template <typename T, typename PartialTraits = std::allocator_traits<std::allocator<T>>>
    class verifying_allocator
      : public allocator_with_id<T, PartialTraits>,
        private verifying_allocator_base
    {
      using base = allocator_with_id<T, PartialTraits>;
      using alloc_traits = std::allocator_traits<base>;

    public:
      verifying_allocator (void) = default;

      constexpr explicit
      verifying_allocator (int id) noexcept
        : base (id)
      { }

      template <typename U>
      constexpr
      verifying_allocator (const verifying_allocator<U, PartialTraits>& other) noexcept
        : base (other)
      { }

      typename alloc_traits::pointer
      allocate (typename alloc_traits::size_type n)
      {
        typename alloc_traits::pointer ret = base::allocate (n);
        try
        {
          register_allocation (*this, ret, n);
        }
        catch (...)
        {
          base::deallocate (ret, n);
          throw;
        }
        return ret;
      }

      void
      deallocate(typename alloc_traits::pointer p, typename alloc_traits::size_type n) noexcept
      {
        assert (nullptr != p);
        remove_allocation (*this, p, n);
        base::deallocate (p, n);
      }
    };

    template <typename T, typename SizeType>
    struct sized_allocator
      : base_allocator<T, allocator_size_type_trait<SizeType>>
    {
      sized_allocator (void) = default;

      template <typename U>
      constexpr
      sized_allocator (const sized_allocator<U, SizeType>&) noexcept
      { }
    };

    template <typename T>
    struct fancy_pointer_allocator
      : base_allocator<T, allocator_pointer_trait<pointer_wrapper<T>>>
    {
      fancy_pointer_allocator (void) = default;

      template <typename U>
      constexpr
      fancy_pointer_allocator (const fancy_pointer_allocator<U>&) noexcept
      { }
    };

  }

}

#endif // SMALL_VECTOR_TEST_ALLOCATORS_HPP
