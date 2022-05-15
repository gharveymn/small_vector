---
title: crend
parent: small_vector
---

# crend

Retrieve a const reverse iterator with the beginning of the container as a base.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
const_iterator
crend (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
const_iterator
crend (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
const_iterator
crend (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
