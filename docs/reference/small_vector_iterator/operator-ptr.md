---
title: operator->
parent: small_vector_iterator
---

# operator->

Retrieve a pointer to the element associated with this iterator.

{% capture unified %}
constexpr
pointer
operator-> (void) const noexcept;
{% endcapture %}

{% include prototype.html unified=unified %}
