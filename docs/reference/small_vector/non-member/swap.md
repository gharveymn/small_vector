---
title: swap
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::swap

Swap the elements between the two containers.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator&gt;
<span class="cpp20">constexpr</span>
void
swap (small_vector<T, InlineCapacity, Allocator>& lhs,
      small_vector<T, InlineCapacity, Allocator>& rhs)
  noexcept (noexcept (lhs.swap (rhs)))<span class="cpp20">
  requires MoveInsertable<T, small_vector<T, InlineCapacity, Allocator&gt;, Allocator&gt;
        && Swappable<T&gt;</span>;
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
constexpr
void
swap (small_vector<T, InlineCapacity, Allocator>& lhs,
      small_vector<T, InlineCapacity, Allocator>& rhs)
  noexcept (noexcept (lhs.swap (rhs)))
  requires MoveInsertable<T, small_vector<T, InlineCapacity, Allocator>, Allocator>
        && Swappable<T>;
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
void
swap (small_vector<T, InlineCapacity, Allocator>& lhs,
      small_vector<T, InlineCapacity, Allocator>& rhs)
  noexcept (noexcept (lhs.swap (rhs)));
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
