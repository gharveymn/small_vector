---
title: erase
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::erase

Erase elements which match a specified value.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator, typename U&gt;
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator&gt;::size_type
erase (small_vector<T, InlineCapacity, Allocator&gt;& v, const U& value);
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator, typename U>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::size_type
erase (small_vector<T, InlineCapacity, Allocator>& v, const U& value);
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator, typename U>
typename small_vector<T, InlineCapacity, Allocator>::size_type
erase (small_vector<T, InlineCapacity, Allocator>& v, const U& value);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
