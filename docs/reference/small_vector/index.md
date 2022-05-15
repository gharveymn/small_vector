---
title: small_vector
has_children: true
nav_order: 0
---

# gch::small_vector

A vector container with a small buffer optimization.

{% capture unified %}
template <typename T, unsigned InlineCapacity, typename Allocator><span class="cpp20">
requires concepts::AllocatorFor<Allocator, T></span>
class small_vector;
{% endcapture %}

{% capture cpp20 %}
template <typename T, unsigned InlineCapacity, typename Allocator>
requires concepts::AllocatorFor<Allocator, T>
class small_vector;
{% endcapture %}

{% capture cpp11 %}
template <typename T, unsigned InlineCapacity, typename Allocator>
class small_vector;
{% endcapture %}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
