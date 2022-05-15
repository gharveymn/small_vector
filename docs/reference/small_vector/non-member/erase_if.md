---
title: erase_if
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::erase_if

Erase elements which cause a specified predicate to return `true`.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator, typename P&gt;
<span class="cpp20">constexpr</span>
typename small_vector<T, InlineCapacity, Allocator&gt;::size_type
erase_if (small_vector<T, InlineCapacity, Allocator&gt;& v, P pred);
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator, typename P>
constexpr
typename small_vector<T, InlineCapacity, Allocator>::size_type
erase_if (small_vector<T, InlineCapacity, Allocator>& v, P pred);
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator, typename P>
typename small_vector<T, InlineCapacity, Allocator>::size_type
erase_if (small_vector<T, InlineCapacity, Allocator>& v, P pred);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
