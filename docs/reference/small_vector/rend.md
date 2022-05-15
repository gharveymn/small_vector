---
title: rend
parent: small_vector
---

# rend

Retrieve a reverse iterator with the beginning of the container as a base.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
iterator
rend (void) noexcept;

<span class="cpp20">constexpr</span>
const_iterator
rend (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
iterator
rend (void) noexcept;

constexpr
const_iterator
rend (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
iterator
rend (void) noexcept;

const_iterator
rend (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
