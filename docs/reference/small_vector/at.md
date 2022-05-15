---
title: at
parent: small_vector
---

# at

Retrieve a reference to the element at the specified index with bounds-checking.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
reference
at (size_type pos);

<span class="cpp20">constexpr</span>
const_reference
at (size_type pos) const;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
reference
at (size_type pos);

constexpr
const_reference
at (size_type pos) const;
{%- endcapture -%}

{%- capture cpp11 -%}
reference
at (size_type pos);

const_reference
at (size_type pos) const;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}