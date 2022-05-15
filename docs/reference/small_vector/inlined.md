---
title: inlined
parent: small_vector
---

# inlined

Query whether the container is using inline storage to store the elements.

{%- capture unified -%}
<span class="cpp17">[[nodiscard]]</span> <span class="cpp20">constexpr</span>
bool
inlined (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
[[nodiscard]] constexpr
bool
inlined (void) const noexcept;
{%- endcapture -%}

{%- capture cpp17 -%}
[[nodiscard]]
bool
inlined (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
bool
inlined (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp17=cpp17 cpp11=cpp11 %}
