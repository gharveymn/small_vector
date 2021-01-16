# small_vector
An implementation of small_vector (vector with a small buffer optimization). No dependencies. 
That was my main gripe with `boost::container::small_vector` and `llvm::SmallVector`. 

Performance is about on par with `boost::container::small_vector` from the small amount of 
testing I've done so far. This implementation also tries to mimic `std::vector` as much as 
possible in terms of top level member functions to allow drop-in replacement.

Compatible with C++11 and up, with `constexpr` and `concept` support for C++20.

# Technical Overview
A `small_vector` is a contiguous sequence container with a certain amount of dedicated 
storage on the stack.  When this storage is filled up, it switches to allocating on the heap.

`small_vector` supports insertion and erasure operations in 𝓞(1) the end, and 𝓞(n) in the 
middle. It also meets the requirements of *Container*, *AllocatorAwareContainer*, 
*SequenceContainer*, *ContiguousContainer*, and *ReversibleContainer*.

When compiling with C++20 support, `small_vector` may be used in `constexpr` expressions.

Template arguments may be used to define the type of stored elements, the number of elements to be 
stored on the stack, and the type of allocator to be used.

# Usage
This is a single header library, so the simplest method of usage is just to drop  
`source/include/gch/small_vector.hpp` and `docs/LICENSE` into your project.

If you prefer CMake, you can first add it as a git submodule with

```commandline
git submodule add -b main git@github.com:gharveymn/small_vector.git external/small_vector
git submodule init external/small_vector
```

Then add it as a subdirectory to your project by adding something like 

```cmake
ADD_SUBDIRECTORY (external/small_vector)
TARGET_LINK_LIBRARIES (<target> gch::small_vector)
```

to `CMakeLists.txt`. Then, you may include the header with 

```c++
#include <gch/small_vector.hpp>
```

in your source.

# Q&A
## Can I specify the `size_type` like with `folly::small_vector`?
Not directly no, but `gch::small_vector::size_type` is derived from `std::allocator_traits`,
so 
you 
can just 
write a wrapper around whatever allocator you're using to modify it. This is as easy as

```c++
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
}

/* ... */

small_vector<int, default_buffer_size_v<my_allocator<int>>, my_allocator<int>> v;
```

where we inherit from `std::allocator` to take advantage of EBO.

## I can't use this with my STL container template templates.
You can create a homogeneous template wrapper with something like 

```c++
template <typename T, typename Allocator = std::allocator<T>, 
          typename InlineCapacityType = 
            std::integral_constant<unsigned, default_buffer_size<Allocator>::value>>
using small_vector_ht = small_vector<T, InlineCapacityType::value, Allocator>;

template <template <typename ...> class VectorT>
void f (void)
{
  VectorT<int> x;
  VectorT<std::string> y;
  /* ... */
}

void g (void)
{
  f<std::vector> ();
  f<small_vector_ht> ();
}

```

I don't include this in the header because too much choice can often cause confusion about how 
things are supposed to be used.

## How do I use this in a `constexpr`?

In C++20, just as with `std::vector`, you cannot create a `constexpr` object of type 
`small_vector`. However you can use it *inside* a `constexpr`. That is,

```c++
// allowed
constexpr int f (void)
{
  small_vector<int> v { 1, 2, 3 };
  return std::accumulate (v.begin (), v.end (), 0);
}

// not allowed
// constexpr small_vector<int> w { };
```

## How do I disable the `concept`s?
You can define the preprocessor directive `GCH_DISABLE_CONCEPTS` with your compiler. These are a 
bit experimental at the moment, so if something is indeed incorrect please feel free to send me a 
note to fix it.

# Brief

In the interest of succinctness, this brief is prepared with declaration decorations compatible 
with C++20. The `constexpr` and `concept` features will not be available for other standards. 

Also note that I've omitted the namespacing and template arguments in the `concept`s  used in 
most of the `requires` statements. Those arguments involve `value_type` and `small_vector`, in the 
obvious fashion.

```c++
namespace gch
{
  namespace concepts
  {
    template <typename T> concept Destructible;
    template <typename T> concept MoveAssignable;
    template <typename T> concept CopyAssignable;
    template <typename T> concept MoveConstructible;
    template <typename T> concept CopyConstructible;
    template <typename T> concept Swappable;

    template <typename T, typename X, typename A, typename ...Args> 
    concept EmplaceConstructible;
    
    template <typename T, typename X> concept DefaultInsertable;
    template <typename T, typename X> concept MoveInsertable;
    template <typename T, typename X> concept CopyInsertable;
    template <typename T, typename X> concept Erasable;
    
    template <typename A> concept Allocator;
  }
  
  /// class used to calculate the default number of elements in inline storage using a heuristic
  template <typename Allocator>
  struct default_buffer_size;

  /// a contiguous iterator (just a pointer wrapper)
  template <typename Pointer, typename DifferenceType>
  class small_vector_iterator;

  template <typename T,
            unsigned InlineCapacity = default_buffer_size<std::allocator<T>>::value,
            typename Allocator      = std::allocator<T>>
    requires concepts::Allocator<Allocator>          
  class small_vector
  {
  public:
    using value_type             = T;
    using allocator_type         = Allocator;
    using size_type              = typename std::allocator_traits<Allocator>::size_type;
    using difference_type        = /* min { signed size_type, alloc_traits::difference_type } */;
    using reference              =       value_type&;
    using const_reference        = const value_type&;
    using pointer                = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer          = typename std::allocator_traits<allocator_type>::const_pointer;
    
    using iterator               = small_vector_iterator<pointer, difference_type>;
    using const_iterator         = small_vector_iterator<const_pointer, difference_type>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /* construction */
    constexpr 
    small_vector (void)
      noexcept (noexcept (allocator_type ()))
      requires DefaultConstructible<allocator_type>;
    
    constexpr 
    small_vector (const small_vector& other)
      requires CopyInsertable && CopyAssignable;

    constexpr 
    small_vector (small_vector&& other) noexcept
      requires MoveInsertable;

    constexpr explicit
    small_vector (const allocator_type& alloc) noexcept;

    constexpr 
    small_vector (const small_vector& other, const allocator_type& alloc)
      requires CopyInsertable;

    constexpr 
    small_vector (small_vector&& other, const allocator_type& alloc)
      requires MoveInsertable;

    constexpr explicit 
    small_vector (size_type count, const allocator_type& alloc = allocator_type ())
      requires DefaultInsertable;

    constexpr explicit 
    small_vector (size_type count, const_reference value, 
                  const allocator_type& alloc = allocator_type ())
      requires CopyInsertable;

    template <std::input_iterator InputIt>
    constexpr 
    small_vector (InputIt first, InputIt last, const allocator_type& alloc = allocator_type ())
      requires EmplaceConstructible<decltype (*first)>::value
           &&  (std::forward_iterator<InputIt> || MoveInsertable);
    
    constexpr 
    small_vector (std::initializer_list<value_type> init,
                  const allocator_type& alloc = allocator_type ())
      requires EmplaceConstructible<const_reference>::value;
    
    /* destruction */
    constexpr
    ~small_vector (void)
      requires Erasable;
    
    /* assignment */
    constexpr 
    small_vector& 
    operator= (const small_vector& other)
      requires CopyInsertable && CopyAssignable;
    
    constexpr 
    small_vector& 
    operator= (small_vector&& other) noexcept
      requires MoveInsertable && MoveAssignable;
    
    constexpr 
    small_vector& 
    operator= (std::initializer_list<value_type> ilist)
      requires CopyInsertable && CopyAssignable;
    
    constexpr 
    void 
    assign (size_type count, const_reference value)
      requires CopyInsertable && CopyAssignable;
    
    template <std::input_iterator InputIt>
    constexpr 
    void 
    assign (InputIt first, InputIt last)
      requires EmplaceConstructible<decltype (*first)>::value
           &&  (std::forward_iterator<InputIt> || MoveInsertable);
    
    constexpr 
    void 
    assign (std::initializer_list<value_type> ilist)
      requires EmplaceConstructible<decltype (*std::begin (ilist))>::value;
    
    constexpr 
    void 
    swap (small_vector& other)
      noexcept (  (  std::allocator_traits<allocator_type>::propagate_on_container_swap::value
                 ||  std::allocator_traits<allocator_type>::is_always_equal::value)
              &&  std::is_nothrow_swappable<value_type>::value
              &&  std::is_nothrow_move_constructible<value_type>::value)
      requires MoveInsertable && Swappable;
    
    /* iteration */
    constexpr iterator               begin   (void)       noexcept;
    constexpr const_iterator         begin   (void) const noexcept;
    constexpr const_iterator         cbegin  (void) const noexcept;
    
    constexpr iterator               end     (void)       noexcept;
    constexpr const_iterator         end     (void) const noexcept;
    constexpr const_iterator         cend    (void) const noexcept;
    
    constexpr reverse_iterator       rbegin  (void)       noexcept;
    constexpr const_reverse_iterator rbegin  (void) const noexcept;
    constexpr const_reverse_iterator crbegin (void) const noexcept;
    
    constexpr reverse_iterator       rend    (void)       noexcept;
    constexpr const_reverse_iterator rend    (void) const noexcept;
    constexpr const_reverse_iterator crend   (void) const noexcept;
    
    /* access */
    constexpr reference       at (size_type pos);
    constexpr const_reference at (size_type pos) const;
    
    constexpr reference       operator[] (size_type pos)       noexcept;
    constexpr const_reference operator[] (size_type pos) const noexcept;
    
    constexpr reference       front (void)       noexcept;
    constexpr const_reference front (void) const noexcept;
    
    constexpr reference       back  (void)       noexcept;
    constexpr const_reference back  (void) const noexcept;
    
    constexpr pointer         data  (void)       noexcept;
    constexpr const_pointer   data  (void) const noexcept;
    
    /* state information */
    [[nodiscard]] 
    constexpr bool empty (void) const noexcept;
    
    constexpr size_type      size          (void) const noexcept;
    constexpr size_type      max_size      (void) const noexcept;
    constexpr size_type      capacity      (void) const noexcept;
    constexpr allocator_type get_allocator (void) const noexcept;
    
    /* insertion */
    constexpr
    iterator
    insert (const_iterator pos, const_reference value)
      requires CopyInsertable && CopyAssignable;
    
    constexpr
    iterator
    insert (const_iterator pos, value_type&& value)
      requires MoveInsertable && MoveAssignable;
    
    constexpr
    iterator
    insert (const_iterator pos, size_type count, const_reference value)
      requires CopyInsertable && CopyAssignable;
    
    template <std::input_iterator InputIt>
    constexpr
    iterator
    insert (const_iterator pos, InputIt first, InputIt last)
      requires EmplaceConstructible<decltype (*first)>::value
           &&  MoveInsertable
           &&  MoveConstructible
           &&  MoveAssignable
           &&  Swappable;
    
    
    constexpr
    iterator
    insert (const_iterator pos, std::initializer_list<value_type> ilist)
      requires EmplaceConstructible<const_reference>::value
           &&  MoveInsertable
           &&  MoveConstructible
           &&  MoveAssignable
           &&  Swappable;
    
    template <typename ...Args>
    constexpr
    iterator
    emplace (const_iterator pos, Args&&... args)
      requires EmplaceConstructible<Args...>::value
           &&  MoveInsertable
           &&  MoveAssignable;
    
    constexpr
    iterator
    erase (const_iterator pos)
      requires MoveAssignable && Erasable;
    
    constexpr
    iterator
    erase (const_iterator first, const_iterator last)
      requires MoveAssignable && Erasable;
    
    constexpr
    void
    push_back (const_reference value)
      requires CopyInsertable;
    
    constexpr
    void
    push_back (value_type&& value)
      requires MoveInsertable;
    
    template <typename ...Args>
    constexpr
    reference
    emplace_back (Args&&... args)
      requires EmplaceConstructible<Args...>::value && MoveInsertable;
    
    constexpr
    void
    pop_back (void)
      requires Erasable;
    
    /* global state modification */
    constexpr
    void 
    reserve (size_type new_cap)
      requires MoveInsertable;
    
    constexpr
    void 
    shrink_to_fit (void)
      requires MoveInsertable;
    
    constexpr
    void 
    clear (void) noexcept
      requires Erasable;
    
    constexpr
    void 
    resize (size_type count)
      requires MoveInsertable && DefaultInsertable;
    
    constexpr 
    void 
    resize (size_type count, const_reference value)
      requires CopyInsertable;
    
    /* non-standard */
    [[nodiscard]] constexpr bool      inlined         (void) const noexcept;
    [[nodiscard]] constexpr bool      inlinable       (void) const noexcept;
    [[nodiscard]] constexpr size_type inline_capacity (void) const noexcept;
  };
  
  /* non-member functions */
  
  template <typename T, unsigned InlineCapacity, typename Allocator>
  constexpr
  bool
  operator== (const small_vector<T, InlineCapacity, Allocator>& lhs,
              const small_vector<T, InlineCapacity, Allocator>& rhs)
  {
    return lhs.size () == rhs.size () && std::equal (lhs.begin (), lhs.end (),
                                                     rhs.begin (), rhs.end ());
  }
  
  template <typename T, unsigned InlineCapacity, typename Allocator>
  constexpr
  auto
  operator<=> (const small_vector<T, InlineCapacity, Allocator>& lhs,
               const small_vector<T, InlineCapacity, Allocator>& rhs);
  
  /* insert other comparison boilerplate here if not using C++20 */
  
  template <typename T, unsigned InlineCapacity, typename Allocator>
  constexpr
  void
  swap (small_vector<T, InlineCapacity, Allocator>& lhs,
        small_vector<T, InlineCapacity, Allocator>& rhs)
    noexcept (noexcept (lhs.swap (rhs)))
    requires MoveInsertable && Swappable;

  template <typename T, unsigned InlineCapacity, typename Allocator, typename U>
  constexpr
  typename small_vector<T, InlineCapacity, Allocator>::size_type
  erase (small_vector<T, InlineCapacity, Allocator>& c, const U& value);

  template <typename T, unsigned InlineCapacity, typename Allocator, typename Pred>
  constexpr
  typename small_vector<T, InlineCapacity, Allocator>::size_type
  erase_if (small_vector<T, InlineCapacity, Allocator>& c, Pred pred);
  
  template <typename InputIt,
            unsigned InlineCapacity = default_buffer_size<
              std::allocator<typename std::iterator_traits<InputIt>::value_type>>::value,
            typename Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>>
  small_vector (InputIt, InputIt, Allocator = Allocator ())
    -> small_vector<typename std::iterator_traits<InputIt>::value_type, InlineCapacity, Allocator>;
}
```
