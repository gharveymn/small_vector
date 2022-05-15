---
title: resize
parent: small_vector
---

# resize

Resize the container by creating or destroying elements.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
void
resize (size_type count);

<span class="cpp20">constexpr</span>
void
resize (size_type count, const_reference value);
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
void
resize (size_type count);

constexpr
void
resize (size_type count, const_reference value);
{%- endcapture -%}

{%- capture cpp11 -%}
void
resize (size_type count);

void
resize (size_type count, const_reference value);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
