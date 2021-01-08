# small_vector
An implementation of small_vector (vector with a small buffer optimization). No dependencies. 
That was my main gripe with `boost::container::small_vector` and `llvm::SmallVector`. 
Performance is about on par with `boost::container::small_vector` from the small amount of 
testing I've done so far. This implementation also tries to mimic `std::vector` as much as 
possible in terms of top level member functions to allow drop-in replacement.

# Q&A
## Can I specify the `size_type` like with `folly::small_vector`?
Not directly no, but here `size_type` is derived from `std::allocator_traits`, so you can just 
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

// ...

using namespace gch;
small_vector<int, default_buffer_size_v<my_allocator<int>>, my_allocator<int>> v;
```

where we inherit from `std::allocator` to take advantage of EBO.
