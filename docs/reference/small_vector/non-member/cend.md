---
title: cend
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::cend

Retrieve a const iterator one past the last element in the container.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator>::const_iterator
cend (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::const_iterator
cend (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
typename small_vector<T, InlineCapacity, Allocator>::const_iterator
cend (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
