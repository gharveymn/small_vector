---
title: back
parent: small_vector
---

# back

Retrieve a reference to the last element in the container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
reference
back (void);

<span class="cpp20">constexpr</span>
const_reference
back (void) const;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
reference
back (void);

constexpr
const_reference
back (void) const;
{%- endcapture -%}

{%- capture cpp11 -%}
reference
back (void);

const_reference
back (void) const;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
