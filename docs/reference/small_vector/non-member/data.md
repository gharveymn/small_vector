---
title: data
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::data

Retrieve a raw pointer to the beginning of the underlying memory range.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator&gt;
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator&gt;::pointer
data (small_vector<T, InlineCapacity, Allocator&gt;& v) noexcept;

template <typename T, unsigned InlineCapacity, typename Allocator&gt;
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator&gt;::const_pointer
data (const small_vector<T, InlineCapacity, Allocator&gt;& v) noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::pointer
data (small_vector<T, InlineCapacity, Allocator>& v) noexcept;

template <typename T, unsigned InlineCapacity, typename Allocator>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::const_pointer
data (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
typename small_vector<T, InlineCapacity, Allocator>::pointer
data (small_vector<T, InlineCapacity, Allocator>& v) noexcept;

template <typename T, unsigned InlineCapacity, typename Allocator>
typename small_vector<T, InlineCapacity, Allocator>::const_pointer
data (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
