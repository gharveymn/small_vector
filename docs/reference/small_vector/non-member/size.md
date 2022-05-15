---
title: size
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::size

Retrieve the number of elements in the container.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator&gt;
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator&gt;::size_type
size (const small_vector<T, InlineCapacity, Allocator&gt;& v) noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::size_type
size (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
typename small_vector<T, InlineCapacity, Allocator>::size_type
size (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
