---
title: reserve
parent: small_vector
---

# reserve

Reserve memory which is sufficient to hold the specified number of elements.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
void
reserve (size_type new_capacity);
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
void
reserve (size_type new_capacity);
{%- endcapture -%}

{%- capture cpp11 -%}
void
reserve (size_type new_capacity);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
