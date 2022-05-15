---
title: operator==
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::operator==

Perform an equality comparison between two containers.

{%- capture unified -%}
template <typename T, unsigned I, unsigned J, typename Allocator&gt;
<span class="cpp20">constexpr</span>
bool
operator== (const small_vector<T, I, Allocator&gt;& lhs,
            const small_vector<T, J, Allocator&gt;& rhs);
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned I, unsigned J, typename Allocator>
constexpr
bool
operator== (const small_vector<T, I, Allocator>& lhs,
            const small_vector<T, J, Allocator>& rhs);
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned I, unsigned J, typename Allocator>
bool
operator== (const small_vector<T, I, Allocator>& lhs,
            const small_vector<T, J, Allocator>& rhs);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
