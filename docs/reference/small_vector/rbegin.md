---
title: rbegin
parent: small_vector
---

# rbegin

Retrieve a reverse iterator with the end of the container as a base.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
iterator
rbegin (void) noexcept;

<span class="cpp20">constexpr</span>
const_iterator
rbegin (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
iterator
rbegin (void) noexcept;

constexpr
const_iterator
rbegin (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
iterator
rbegin (void) noexcept;

const_iterator
rbegin (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
