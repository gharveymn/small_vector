---
title: emplace
parent: small_vector
---

# emplace

Construct a new element at the specified location in the container with the specified arguments.

{%- capture unified -%}
template <typename ...Args&gt;<span class="cpp20">
requires EmplaceConstructible<Args...&gt;::value
     &&  MoveInsertable
     &&  MoveAssignable
constexpr</span>
iterator
emplace (const_iterator pos, Args&&... args);
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename ...Args>
requires EmplaceConstructible<Args...>::value
     &&  MoveInsertable
     &&  MoveAssignable
constexpr
iterator
emplace (const_iterator pos, Args&&... args);
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename ...Args>
iterator
emplace (const_iterator pos, Args&&... args);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
