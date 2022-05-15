---
title: operator<=>
parent: Non-Member Functions
grand_parent: small_vector_iterator
---

# gch::operator<=> <span class="title-annotation">(since C++20)</span>

Perform an three-way comparison between two iterators.

{%- capture unified -%}
template <typename PointerLHS, typename DifferenceTypeLHS,
          typename PointerRHS, typename DifferenceTypeRHS>
constexpr
auto
operator<=> (const small_vector_iterator<PointerLHS, DifferenceTypeLHS>& lhs,
             const small_vector_iterator<PointerRHS, DifferenceTypeRHS>& rhs);
{%- endcapture -%}

{% include prototype.html unified=unified since=20 %}
