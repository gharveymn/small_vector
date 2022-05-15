---
title: begin
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::begin

Retrieve an iterator pointing to the first element in the container.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator&gt;
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator&gt;::iterator
begin (small_vector<T, InlineCapacity, Allocator&gt;& v) noexcept;

template <typename T, unsigned InlineCapacity, typename Allocator&gt;
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator&gt;::const_iterator
begin (const small_vector<T, InlineCapacity, Allocator&gt;& v) noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::iterator
begin (small_vector<T, InlineCapacity, Allocator>& v) noexcept;

template <typename T, unsigned InlineCapacity, typename Allocator>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::const_iterator
begin (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
typename small_vector<T, InlineCapacity, Allocator>::iterator
begin (small_vector<T, InlineCapacity, Allocator>& v) noexcept;

template <typename T, unsigned InlineCapacity, typename Allocator>
typename small_vector<T, InlineCapacity, Allocator>::const_iterator
begin (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
