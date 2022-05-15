---
title: operator[]
parent: small_vector
---

# operator[]

Retrieve a reference to the element at the specified index without bounds-checking.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
reference
operator[] (size_type pos);

<span class="cpp20">constexpr</span>
const_reference
operator[] (size_type pos) const;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
reference
operator[] (size_type pos);

constexpr
const_reference
operator[] (size_type pos) const;
{%- endcapture -%}

{%- capture cpp11 -%}
reference
operator[] (size_type pos);

const_reference
operator[] (size_type pos) const;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
