---
title: shrink_to_fit
parent: small_vector
---

# shrink_to_fit

Reduce the capacity of the container to the minimum required to store the elements.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
void
shrink_to_fit (void);
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
void
shrink_to_fit (void);
{%- endcapture -%}

{%- capture cpp11 -%}
void
shrink_to_fit (void);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
