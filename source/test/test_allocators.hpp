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
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
    -> decltype (gch::test_types::to_address (p.operator-> ()))
    {
      return gch::test_types::to_address (p.operator-> ());
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
      template <typename U, typename Traits, typename Enable = void>
      struct rebind_traits
      {
        using type = Traits;
      };

      template <typename U, typename Traits>
      struct rebind_traits<
        U,
        Traits,
        void_t<typename Traits::template rebind_traits<U>>>
      {
        using type = typename Traits::template rebind_traits<U>;
      };

      using traits = typename rebind_traits<T, PartialTraits>::type;

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
      using difference_type = typename extracted_difference_type<traits>::type;
      using size_type = typename extracted_size_type<traits>::type;
      using pointer = typename extracted_pointer_type<traits>::type;
      using propagate_on_container_move_assignment = std::true_type;
      using is_always_equal = std::true_type;

      base_allocator (void) = default;

      template <typename U>
      constexpr GCH_IMPLICIT_CONVERSION
      base_allocator (const base_allocator<U, PartialTraits>&) noexcept
      { }

      GCH_NODISCARD GCH_CPP20_CONSTEXPR
      pointer
      allocate (size_type n)
      {
        return pointer (std::allocator<value_type> ().allocate (n));
      }

      GCH_CPP20_CONSTEXPR
      void
      deallocate (pointer p, size_type n) noexcept
      {
        std::allocator<value_type> ().deallocate (gch::test_types::to_address (p), n);
      }
    };

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator== (const base_allocator<T, Traits>&, const base_allocator<U, Traits>&) noexcept
    {
      return true;
    }

    template <typename T, typename Traits>
    constexpr
    bool
    operator!= (const base_allocator<T, Traits>&, const base_allocator<T, Traits>&) noexcept
    {
      return false;
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
      constexpr GCH_IMPLICIT_CONVERSION
      allocator_with_id (const allocator_with_id<U, PartialTraits>& other) noexcept
        : base (other),
          m_id (other.get_id ())
      { }

      GCH_NODISCARD constexpr
      int
      get_id (void) const noexcept
      {
        return m_id;
      }

    private:
      int m_id = 0;
    };

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator== (const allocator_with_id<T, Traits>& lhs,
                const allocator_with_id<U, Traits>& rhs) noexcept
    {
      return lhs.get_id () == rhs.get_id ();
    }

    template <typename T, typename Traits>
    constexpr
    bool
    operator!= (const allocator_with_id<T, Traits>& lhs,
                const allocator_with_id<T, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator!= (const allocator_with_id<T, Traits>& lhs,
                const allocator_with_id<U, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    struct verifying_allocator_base
    {
      using allocation_tracker_type = std::unordered_map<void *, std::size_t>;
      using object_tracker_type = std::unordered_set<void *>;

      struct data_trackers {
        std::unordered_map<void *, std::size_t> m_allocations;
        std::unordered_set<void *> m_objects;
      };

      using allocator_map_type = std::unordered_map<int, data_trackers>;

      static
      allocator_map_type&
      get_map (void)
      {
        auto& stack = allocator_map_stack ();
        if (stack.empty ())
          push_context();
        return *stack.top();
      }

      static
      void
      push_context ()
      {
        allocator_map_stack ().push (std::unique_ptr<allocator_map_type, allocator_map_deleter> {
          new allocator_map_type
        });
      }

      static
      void
      pop_context ()
      {
        allocator_map_stack ().pop ();
      }

      template <typename Functor>
      static
      void
      with_scoped_context (Functor f)
      {
        push_context ();
        f ();
        pop_context ();
      }

      template <typename T, typename Traits>
      static
      allocation_tracker_type&
      get_allocation_tracker (const allocator_with_id<T, Traits>& alloc)
      {
        allocator_map_type& map = get_map();
        auto allocator_it = map.find (alloc.get_id ());
        assert (allocator_it != map.end () && "The allocator has not been used yet.");
        return allocator_it->second.m_allocations;
      }

      template <typename T, typename Traits>
      static
      object_tracker_type&
      get_object_tracker (const allocator_with_id<T, Traits>& alloc)
      {
        allocator_map_type& map = get_map();
        auto allocator_it = map.find (alloc.get_id ());
        assert (allocator_it != map.end () && "The allocator has not been used yet.");
        return allocator_it->second.m_objects;
      }

      template <typename T, typename Traits, typename Pointer>
      static
      void
      register_allocation (const allocator_with_id<T, Traits>& alloc, Pointer p, std::size_t n)
      {
        allocation_tracker_type& tkr = get_map ()[alloc.get_id ()].m_allocations;
        auto emplace_pair = tkr.emplace (static_cast<void *> (gch::test_types::to_address (p)), n);
        assert (emplace_pair.second && "Allocation overwritten.");
      }

      template <typename T, typename Traits, typename Pointer>
      static
      allocation_tracker_type::const_iterator
      verify_allocation (const allocator_with_id<T, Traits>& alloc, Pointer p, std::size_t n)
      {
        const allocation_tracker_type& tkr = get_allocation_tracker (alloc);

        auto it = tkr.find (static_cast<void *> (gch::test_types::to_address (p)));
        assert (it != tkr.end () && "Could not find the allocation for the given allocator.");

        assert (it->second == n && "The allocation size did not match the given size.");
        return it;
      }

      template <typename T, typename Traits, typename Pointer>
      static
      void
      remove_allocation (const allocator_with_id<T, Traits>& alloc, Pointer p, std::size_t n)
      {
        allocation_tracker_type& tkr = get_allocation_tracker (alloc);
        tkr.erase (verify_allocation (alloc, p, n));
      }

      template <typename T, typename Traits, typename Pointer>
      static
      void
      register_object (const allocator_with_id<T, Traits>& alloc, Pointer p)
      {
        object_tracker_type& tkr = get_map ()[alloc.get_id ()].m_objects;
        auto emplace_pair = tkr.emplace (static_cast<void *> (gch::test_types::to_address (p)));
        assert (emplace_pair.second && "Object overwritten.");
      }

      template <typename T, typename Traits, typename Pointer>
      static
      object_tracker_type::const_iterator
      verify_object (const allocator_with_id<T, Traits>& alloc, Pointer p)
      {
        const object_tracker_type& tkr = get_object_tracker (alloc);
        auto it = tkr.find (static_cast<void *> (gch::test_types::to_address (p)));
        assert (it != tkr.end () && "Could not find the object for the given allocator.");
        return it;
      }

      template <typename T, typename Traits, typename Pointer>
      static
      void
      remove_object (const allocator_with_id<T, Traits>& alloc, Pointer p)
      {
        object_tracker_type & tkr = get_object_tracker (alloc);
        tkr.erase (verify_object (alloc, p));
      }

    private:
      struct allocator_map_deleter
      {
        void
        operator() (allocator_map_type *map_ptr) noexcept
        {
          std::for_each (map_ptr->begin (), map_ptr->end (), [](
            const allocator_map_type::value_type& pair
          ) {
            assert (pair.second.m_allocations.empty () && "Some allocations were not freed.");
            assert (pair.second.m_objects.empty () && "Not all objects were destroyed.");
          });
          delete map_ptr;
        }
      };

      static
      std::stack<std::unique_ptr<allocator_map_type, allocator_map_deleter>>&
      allocator_map_stack () noexcept
      {
        static std::stack<std::unique_ptr<allocator_map_type, allocator_map_deleter>> stack;
        return stack;
      }
    };

    template <typename T, typename PartialTraits = std::allocator_traits<std::allocator<T>>,
              typename Copy = std::true_type, typename Move = std::true_type,
              typename Swap = std::true_type>
    class verifying_allocator
      : public allocator_with_id<T, PartialTraits>,
        private verifying_allocator_base
    {
      using base = allocator_with_id<T, PartialTraits>;
      using alloc_traits = std::allocator_traits<base>;

    public:
      using propagate_on_container_copy_assignment = Copy;
      using propagate_on_container_move_assignment = Move;
      using propagate_on_container_swap = Swap;

      verifying_allocator            (void)                           = default;
      verifying_allocator            (const verifying_allocator&)     = default;
      verifying_allocator            (verifying_allocator&&) noexcept = default;
      verifying_allocator& operator= (const verifying_allocator&)     = default;
      verifying_allocator& operator= (verifying_allocator&&) noexcept = default;
      ~verifying_allocator           (void)                           = default;

      constexpr explicit
      verifying_allocator (int id) noexcept
        : base (id)
      { }

      template <typename U>
      constexpr GCH_IMPLICIT_CONVERSION
      verifying_allocator (const verifying_allocator<U, PartialTraits, Copy, Move, Swap>& other)
        noexcept
        : base (other),
          created_by_container_copy_construction (other.created_by_container_copy_construction)
      { }

      GCH_NODISCARD
      typename alloc_traits::pointer
      allocate (typename alloc_traits::size_type n) noexcept (false)
      {
        typename alloc_traits::pointer ret = base::allocate (n);
        GCH_TRY
        {
          register_allocation (*this, ret, n);
        }
        GCH_CATCH (...)
        {
          base::deallocate (ret, n);
          GCH_THROW;
        }
        return ret;
      }

      void
      deallocate (typename alloc_traits::pointer p, typename alloc_traits::size_type n) noexcept
      {
        assert (nullptr != p);
        remove_allocation (*this, p, n);
        base::deallocate (p, n);
      }

      template <typename U, typename ...Args>
      void
      construct (U *p, Args&&... args) noexcept (false)
      {
        alloc_traits::construct (*this, p, std::forward<Args> (args)...);

        GCH_TRY
        {
          register_object (*this, p);
        }
        GCH_CATCH (...)
        {
          alloc_traits::destroy (*this, p);
          GCH_THROW;
        }
      }

      template <typename U>
      void
      destroy (U *p)
      {
        assert (nullptr != p);
        remove_object (*this, p);
        alloc_traits::destroy (*this, p);
      }

      verifying_allocator
      select_on_container_copy_construction () const
      {
        verifying_allocator ret { *this };
        ret.created_by_container_copy_construction = true;
        return ret;
      }

      bool created_by_container_copy_construction = false;
    };

    template <typename A>
    constexpr int
    verify_created_by_container_copy_construction (const A&)
    {
      return 0;
    }

    template <typename T, typename Traits>
    constexpr int
    verify_created_by_container_copy_construction (const verifying_allocator<T, Traits>& a)
    {
      return assert (a.created_by_container_copy_construction
          &&  "select_on_container_copy_construction unexpectedly invoked"), 0;
    }

    template <typename A>
    constexpr int
    verify_not_created_by_container_copy_construction (const A&)
    {
      return 0;
    }

    template <typename T, typename Traits>
    constexpr int
    verify_not_created_by_container_copy_construction (const verifying_allocator<T, Traits>& a)
    {
      return assert (! a.created_by_container_copy_construction
          &&  "select_on_container_copy_construction unexpectedly not invoked"), 0;
    }

    template <typename T, typename Traits>
    constexpr
    bool
    operator!= (const verifying_allocator<T, Traits>& lhs,
                const verifying_allocator<T, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator!= (const verifying_allocator<T, Traits>& lhs,
                const verifying_allocator<U, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <bool Copy = true, bool Move = true, bool Swap = true>
    struct verifying_allocator_with_traits {
      template <typename T, typename Traits = std::allocator_traits<std::allocator<T>>>
      using type = verifying_allocator<
        T,
        Traits,
        std::integral_constant<bool, Copy>,
        std::integral_constant<bool, Move>,
        std::integral_constant<bool, Swap>
      >;
    };

    template <typename T, typename Traits = std::allocator_traits<std::allocator<T>>>
    using non_propagating_verifying_allocator =
      verifying_allocator_with_traits<false, false, false>::type<T, Traits>;

    template <typename T, typename SizeType>
    struct sized_allocator
      : base_allocator<T, allocator_size_type_trait<SizeType>>
    {
      sized_allocator (void) = default;

      template <typename U>
      constexpr GCH_IMPLICIT_CONVERSION
      sized_allocator (const sized_allocator<U, SizeType>&) noexcept
      { }
    };

    template <typename T, typename SizeType>
    constexpr
    bool
    operator!= (const sized_allocator<T, SizeType>& lhs,
                const sized_allocator<T, SizeType>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <typename T, typename U, typename SizeType>
    constexpr
    bool
    operator!= (const sized_allocator<T, SizeType>& lhs,
                const sized_allocator<U, SizeType>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <typename T, typename Traits = allocator_pointer_trait<pointer_wrapper<T>>>
    struct fancy_pointer_allocator
      : base_allocator<T, Traits>
    {
      fancy_pointer_allocator (void) = default;

      template <typename U>
      constexpr GCH_IMPLICIT_CONVERSION
      fancy_pointer_allocator (const fancy_pointer_allocator<U, Traits>&) noexcept
      { }
    };

    template <typename T, typename Traits>
    constexpr
    bool
    operator!= (const fancy_pointer_allocator<T, Traits>& lhs,
                const fancy_pointer_allocator<T, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator!= (const fancy_pointer_allocator<T, Traits>& lhs,
                const fancy_pointer_allocator<U, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <typename T, typename Traits = std::allocator_traits<std::allocator<T>>>
    struct propagating_allocator_with_id
      : allocator_with_id<T, Traits>
    {
      using propagate_on_container_copy_assignment = std::true_type;
      using propagate_on_container_swap = std::true_type;

      propagating_allocator_with_id (void) = default;

      template <typename U>
      constexpr GCH_IMPLICIT_CONVERSION
      propagating_allocator_with_id (const propagating_allocator_with_id<U, Traits>&) noexcept
      { }

      using allocator_with_id<T, Traits>::allocator_with_id;
    };

    template <typename T, typename Traits>
    constexpr
    bool
    operator!= (const propagating_allocator_with_id<T, Traits>& lhs,
                const propagating_allocator_with_id<T, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

    template <typename T, typename U, typename Traits>
    constexpr
    bool
    operator!= (const propagating_allocator_with_id<T, Traits>& lhs,
                const propagating_allocator_with_id<U, Traits>& rhs) noexcept
    {
      return ! (lhs == rhs);
    }

  }

}

#endif // SMALL_VECTOR_TEST_ALLOCATORS_HPP
