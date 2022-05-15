---
title: clear
parent: small_vector
---

# clear

Destroy all elements in the container and set the size to zero.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
void
clear (void) noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
void
clear (void) noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
void
clear (void) noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
