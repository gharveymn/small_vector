---
title: operator-
parent: Non-Member Functions
grand_parent: small_vector_iterator
---

# gch::operator-

Compute the offset between two iterators.

{%- capture unified -%}
template <typename PointerLHS, typename PointerRHS, typename DifferenceType>
constexpr
DifferenceType
operator- (const small_vector_iterator<PointerLHS, DifferenceType>& lhs,
            const small_vector_iterator<PointerRHS, DifferenceType>& rhs)
  noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified until=20 %}
