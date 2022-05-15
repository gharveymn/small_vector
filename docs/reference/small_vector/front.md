---
title: front
parent: small_vector
---

# front

Retrieve a reference to the first element in the container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
reference
front (void);

<span class="cpp20">constexpr</span>
const_reference
front (void) const;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
reference
front (void);

constexpr
const_reference
front (void) const;
{%- endcapture -%}

{%- capture cpp11 -%}
reference
front (void);

const_reference
front (void) const;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
