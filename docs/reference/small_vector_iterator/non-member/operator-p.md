---
title: operator+
parent: Non-Member Functions
grand_parent: small_vector_iterator
---

# gch::operator+

Construct a new iterator located by adding the specified offset to an iterator.

{%- capture unified -%}
template <typename Pointer, typename DifferenceType>
constexpr
small_vector_iterator<Pointer, DifferenceType>
operator+ (DifferenceType n,
           const small_vector_iterator<Pointer, DifferenceType>& it)
  noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified until=20 %}
