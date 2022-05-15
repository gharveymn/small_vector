---
title: emplace_back
parent: small_vector
---

# emplace_back

Construct a new element at the end of the container with the specified arguments.

{%- capture unified -%}
template <typename ...Args&gt;<span class="cpp20">
requires EmplaceConstructible<Args...&gt;::value && MoveInsertable
constexpr</span>
reference
emplace_back (Args&&... args);
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename ...Args>
requires EmplaceConstructible<Args...>::value && MoveInsertable
constexpr
reference
emplace_back (Args&&... args);
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename ...Args>
reference
emplace_back (Args&&... args);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
