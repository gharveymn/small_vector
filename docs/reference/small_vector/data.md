---
title: data
parent: small_vector
---

# data

Retrieve a raw pointer to the beginning of the underlying memory range.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
pointer
data (void);

<span class="cpp20">constexpr</span>
const_pointer
data (void) const;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
pointer
data (void) noexcept;

constexpr
const_pointer
data (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
pointer
data (void) noexcept;

const_pointer
data (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
