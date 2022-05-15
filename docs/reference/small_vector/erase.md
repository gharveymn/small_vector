---
title: erase
parent: small_vector
---

# erase

Destroy the elements in the container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
iterator
erase (const_iterator pos);

<span class="cpp20">constexpr</span>
iterator
erase (const_iterator first, const_iterator last);
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
iterator
erase (const_iterator pos);

constexpr
iterator
erase (const_iterator first, const_iterator last);
{%- endcapture -%}

{%- capture cpp11 -%}
iterator
erase (const_iterator pos);

iterator
erase (const_iterator first, const_iterator last);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
