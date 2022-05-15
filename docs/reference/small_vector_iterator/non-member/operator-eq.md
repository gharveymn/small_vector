---
title: operator==
parent: Non-Member Functions
grand_parent: small_vector_iterator
---

# gch::operator==

Perform an equality comparison between two iterators.

{%- capture unified -%}
template <typename PointerLHS, typename DifferenceTypeLHS,
          typename PointerRHS, typename DifferenceTypeRHS&gt;
constexpr
bool
operator== (const small_vector_iterator<PointerLHS, DifferenceTypeLHS&gt;& lhs,
            const small_vector_iterator<PointerRHS, DifferenceTypeRHS&gt;& rhs)
  noexcept <span class="cpp20">(noexcept (lhs.base () == rhs.base ()))
  requires requires {
    { lhs.base () == rhs.base () } -> std::convertible_to<bool&gt;;
  }</span>;
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename PointerLHS, typename DifferenceTypeLHS,
          typename PointerRHS, typename DifferenceTypeRHS>
constexpr
bool
operator== (const small_vector_iterator<PointerLHS, DifferenceTypeLHS>& lhs,
            const small_vector_iterator<PointerRHS, DifferenceTypeRHS>& rhs)
  noexcept (noexcept (lhs.base () == rhs.base ()))
  requires requires {
    { lhs.base () == rhs.base () } -> std::convertible_to<bool>;
  };
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename PointerLHS, typename DifferenceTypeLHS,
          typename PointerRHS, typename DifferenceTypeRHS>
constexpr
bool
operator== (const small_vector_iterator<PointerLHS, DifferenceTypeLHS>& lhs,
            const small_vector_iterator<PointerRHS, DifferenceTypeRHS>& rhs)
  noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
