---
title: operator+=
parent: small_vector_iterator
---

# operator+=

Add the specified offset to the iterator.

{% capture unified %}
<span class="cpp14">constexpr</span>
small_vector_iterator&
operator+= (difference_type n) noexcept;
{% endcapture %}

{% capture cpp14 %}
constexpr
small_vector_iterator&
operator+= (difference_type n) noexcept;
{% endcapture %}

{% capture cpp11 %}
small_vector_iterator&
operator+= (difference_type n) noexcept;
{% endcapture %}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
