---
title: operator>=
parent: Non-Member Functions
grand_parent: small_vector_iterator
---

# gch::operator>= <span class="title-annotation">(deleted in C++20)</span>

Determine whether the first iterator points to an element at or after the second iterator.

{%- capture unified -%}
template <typename PointerLHS, typename DifferenceTypeLHS,
          typename PointerRHS, typename DifferenceTypeRHS>
constexpr
bool
operator>= (const small_vector_iterator<PointerLHS, DifferenceTypeLHS>& lhs,
            const small_vector_iterator<PointerRHS, DifferenceTypeRHS>& rhs)
  noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified until=20 %}
