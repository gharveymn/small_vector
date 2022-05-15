---
title: operator-
parent: small_vector_iterator
---

# operator-

Construct a new iterator located by subtracting the specified offset.

{% capture unified %}
constexpr
small_vector_iterator
operator- (difference_type n) const noexcept;
{% endcapture %}

{% include prototype.html unified=unified %}
