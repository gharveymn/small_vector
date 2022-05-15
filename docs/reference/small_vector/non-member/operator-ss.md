---
title: operator<=>
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::operator<=> <span class="title-annotation">(since C++20)</span>

Perform an three-way comparison between two containers.

{%- capture unified -%}
template <typename T, unsigned I, unsigned J, typename Allocator>
constexpr
auto
operator<=> (const small_vector<T, I, Allocator>& lhs,
             const small_vector<T, J, Allocator>& rhs);
{%- endcapture -%}

{% include prototype.html unified=unified since=20 %}
