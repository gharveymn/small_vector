---
title: assign
parent: small_vector
---

# assign

Assign a value to the container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
void
assign (size_type count, const_reference value);

template <[<span class="cpp20">std::input_iterator</span> | <span class="cpp11">typename</span>] InputIt&gt;<span class="cpp20">
requires EmplaceConstructible<std::iter_reference_t<InputIt&gt;&gt;
     &&  (std::forward_iterator<InputIt&gt; || MoveInsertable)
constexpr</span>
void
assign (InputIt first, InputIt last);

<span class="cpp20">constexpr</span>
void
assign (std::initializer_list<value_type&gt; ilist);

<span class="cpp20">constexpr</span>
void
assign (const small_vector& other);

template <unsigned I&gt;<span class="cpp20">
requires CopyInsertable && CopyAssignable
constexpr</span>
void
assign (const small_vector<T, I, Allocator&gt;& other);

<span class="cpp20">constexpr</span>
void
assign (small_vector&& other)
  noexcept (/* See below. */);

template <unsigned I&gt;<span class="cpp20">
requires MoveInsertable && MoveAssignable
constexpr</span>
void
assign (small_vector<T, I, Allocator&gt;&& other)
  noexcept (/* See below. */);
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
void
assign (size_type count, const_reference value);

template <std::input_iterator InputIt>
requires EmplaceConstructible<std::iter_reference_t<InputIt>>
     &&  (std::forward_iterator<InputIt> || MoveInsertable)
constexpr
void
assign (InputIt first, InputIt last);

constexpr
void
assign (std::initializer_list<value_type> ilist);

constexpr
void
assign (const small_vector& other);

template <unsigned I>
requires CopyInsertable && CopyAssignable
constexpr
void
assign (const small_vector<T, I, Allocator>& other);

constexpr
void
assign (small_vector&& other)
  noexcept (/* See below. */);

template <unsigned I>
requires MoveInsertable && MoveAssignable
constexpr
void
assign (small_vector<T, I, Allocator>&& other)
  noexcept (/* See below. */);
{%- endcapture -%}

{%- capture cpp11 -%}
void
assign (size_type count, const_reference value);

template <std::input_iterator InputIt>
void
assign (InputIt first, InputIt last);

void
assign (std::initializer_list<value_type> ilist);

void
assign (const small_vector& other);

template <unsigned I>
void
assign (const small_vector<T, I, Allocator>& other);

void
assign (small_vector&& other)
  noexcept (/* See below. */);

template <unsigned I>
void
assign (small_vector<T, I, Allocator>&& other)
  noexcept (/* See below. */);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
