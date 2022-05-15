---
title: swap
parent: small_vector
---

# swap

Swap the elements with those held by another container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
void
swap (small_vector& other)
  noexcept (/* See below. */)<span class="cpp20">
  requires (/* See below. */)</span>;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
void
swap (small_vector& other)
  noexcept (/* See below. */)
  requires (/* See below. */);
{%- endcapture -%}

{%- capture cpp11 -%}
void
swap (small_vector& other)
  noexcept (/* See below. */);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
