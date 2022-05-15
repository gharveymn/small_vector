---
title: operator--
parent: small_vector_iterator
---

# operator--

Decrement the iterator by one.

{% capture unified %}
<span class="cpp14">constexpr</span>
small_vector_iterator&
operator-- (void) noexcept;

<span class="cpp14">constexpr</span>
small_vector_iterator
operator-- (int) noexcept;
{% endcapture %}

{% capture cpp14 %}
constexpr
small_vector_iterator&
operator-- (void) noexcept;

constexpr
small_vector_iterator
operator-- (int) noexcept;
{% endcapture %}

{% capture cpp11 %}
small_vector_iterator&
operator-- (void) noexcept;

small_vector_iterator
operator-- (int) noexcept;
{% endcapture %}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
