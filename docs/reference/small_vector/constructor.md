---
title: small_vector
parent: small_vector
---

# Constructor
{: .body-font }

Construct a `small_vector`.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
small_vector (void)
  noexcept (noexcept (allocator_type ()))<span class="cpp20">
  requires DefaultConstructible<allocator_type&gt;</span>;

<span class="cpp20">constexpr</span>
small_vector (const small_vector& other)<span class="cpp20">
  requires CopyInsertable</span>;

<span class="cpp20">constexpr</span>
small_vector (small_vector&& other)
  noexcept (std::is_nothrow_move_constructible<value_type&gt;::value
        ||  InlineCapacity == 0)<span class="cpp20">
  requires MoveInsertable</span>;

<span class="cpp20">constexpr</span> explicit
small_vector (const allocator_type& alloc) noexcept;

<span class="cpp20">constexpr</span>
small_vector (const small_vector& other, const allocator_type& alloc)<span class="cpp20">
  requires CopyInsertable</span>;

<span class="cpp20">constexpr</span>
small_vector (small_vector&& other, const allocator_type& alloc)<span class="cpp20">
  requires MoveInsertable</span>;

<span class="cpp20">constexpr</span> explicit
small_vector (size_type count, 
              const allocator_type& alloc = allocator_type ())<span class="cpp20">
  requires DefaultInsertable</span>;

<span class="cpp20">constexpr</span>
small_vector (size_type count, const_reference value,
              const allocator_type& alloc = allocator_type ())<span class="cpp20">
  requires CopyInsertable</span>;

template <typename Generator&gt;<span class="cpp20">
requires std::invocable<Generator&&gt;
      &&  EmplaceConstructible<std::invoke_result_t<Generator&&gt;&gt;::value
constexpr</span>
small_vector (size_type count, Generator g,
              const allocator_type& alloc = allocator_type ());

template <[<span class="cpp20">std::input_iterator</span> | <span class="cpp11">typename</span>] InputIt&gt;<span class="cpp20">
requires EmplaceConstructible<std::iter_reference_t<InputIt&gt;&gt;::value
     &&  (std::forward_iterator<InputIt&gt; || MoveInsertable)
constexpr</span>
small_vector (InputIt first, InputIt last,
              const allocator_type& alloc = allocator_type ());

<span class="cpp20">constexpr</span>
small_vector (std::initializer_list<value_type&gt; init,
              const allocator_type& alloc = allocator_type ());

template <unsigned I&gt;<span class="cpp20">
requires CopyInsertable
constexpr</span> explicit
small_vector (const small_vector<T, I, Allocator&gt;& other);

template <unsigned I&gt;<span class="cpp20">
requires MoveInsertable
constexpr</span> explicit
small_vector (small_vector<T, I, Allocator&gt;&& other)
  noexcept (std::is_nothrow_move_constructible<value_type&gt;::value
        &&  I < InlineCapacity);

template <unsigned I&gt;<span class="cpp20">
requires CopyInsertable
constexpr</span>
small_vector (const small_vector<T, I, Allocator&gt;& other,
              const allocator_type& alloc);

template <unsigned I&gt;<span class="cpp20">
requires MoveInsertable
constexpr</span>
small_vector (small_vector<T, I, Allocator&gt;&& other,
              const allocator_type& alloc);
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
small_vector (void)
  noexcept (noexcept (allocator_type ()))
  requires DefaultConstructible<allocator_type>;

constexpr
small_vector (const small_vector& other)
  requires CopyInsertable;

constexpr
small_vector (small_vector&& other)
  noexcept (std::is_nothrow_move_constructible<value_type>::value
        ||  InlineCapacity == 0)
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
small_vector (size_type count,
              const allocator_type& alloc = allocator_type ())
  requires DefaultInsertable;

constexpr
small_vector (size_type count, const_reference value,
              const allocator_type& alloc = allocator_type ())
  requires CopyInsertable;

template <typename Generator>
requires std::invocable<Generator&>
      &&  EmplaceConstructible<std::invoke_result_t<Generator&>>::value
constexpr
small_vector (size_type count, Generator g,
              const allocator_type& alloc = allocator_type ());

template <std::input_iterator InputIt>
requires EmplaceConstructible<std::iter_reference_t<InputIt>>::value
     &&  (std::forward_iterator<InputIt> || MoveInsertable)
constexpr
small_vector (InputIt first, InputIt last,
              const allocator_type& alloc = allocator_type ());

constexpr
small_vector (std::initializer_list<value_type> init,
              const allocator_type& alloc = allocator_type ());

template <unsigned I>
requires CopyInsertable
constexpr explicit
small_vector (const small_vector<T, I, Allocator>& other);

template <unsigned I>
requires MoveInsertable
constexpr explicit
small_vector (small_vector<T, I, Allocator>&& other)
  noexcept (std::is_nothrow_move_constructible<value_type>::value
        &&  I < InlineCapacity);

template <unsigned I>
requires CopyInsertable
constexpr
small_vector (const small_vector<T, I, Allocator>& other,
              const allocator_type& alloc);

template <unsigned I>
requires MoveInsertable
constexpr
small_vector (small_vector<T, I, Allocator>&& other,
              const allocator_type& alloc);
{%- endcapture -%}

{%- capture cpp11 -%}
small_vector (void)
  noexcept (noexcept (allocator_type ()));

small_vector (const small_vector& other);

small_vector (small_vector&& other)
  noexcept (std::is_nothrow_move_constructible<value_type>::value 
        ||  InlineCapacity == 0);

explicit
small_vector (const allocator_type& alloc) noexcept;

small_vector (const small_vector& other, const allocator_type& alloc);

small_vector (small_vector&& other, const allocator_type& alloc);

explicit
small_vector (size_type count,
              const allocator_type& alloc = allocator_type ());

small_vector (size_type count, const_reference value,
              const allocator_type& alloc = allocator_type ());

template <typename Generator>
small_vector (size_type count, Generator g,
              const allocator_type& alloc = allocator_type ());

template <typename InputIt>
small_vector (InputIt first, InputIt last,
              const allocator_type& alloc = allocator_type ());

small_vector (std::initializer_list<value_type> init,
              const allocator_type& alloc = allocator_type ());

template <unsigned I>
explicit
small_vector (const small_vector<T, I, Allocator>& other);

template <unsigned I>
explicit
small_vector (small_vector<T, I, Allocator>&& other)
  noexcept (std::is_nothrow_move_constructible<value_type>::value
        &&  I < InlineCapacity);

template <unsigned I>
small_vector (const small_vector<T, I, Allocator>& other,
              const allocator_type& alloc);

template <unsigned I>
small_vector (small_vector<T, I, Allocator>&& other,
              const allocator_type& alloc);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
