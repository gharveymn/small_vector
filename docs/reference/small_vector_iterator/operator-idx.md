---
title: operator[]
parent: small_vector_iterator
---

# operator[]

Retrieve a reference to an element located at the specified offset relative to the iterator.

{% capture unified %}
constexpr
reference
operator[] (difference_type n) const noexcept;
{% endcapture %}

{% include prototype.html unified=unified %}
