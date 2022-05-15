---
title: empty
parent: small_vector
---

# empty

Query whether the container is empty.

{%- capture unified -%}
<span class="cpp17">[[nodiscard]]</span> <span class="cpp20">constexpr</span>
bool
empty (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
[[nodiscard]] constexpr
bool
empty (void) const noexcept;
{%- endcapture -%}

{%- capture cpp17 -%}
[[nodiscard]]
bool
empty (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
bool
empty (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp17=cpp17 cpp11=cpp11 %}
