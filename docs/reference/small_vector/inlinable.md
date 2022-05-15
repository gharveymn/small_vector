---
title: inlinable
parent: small_vector
---

# inlinable

Query whether the container elements can fit inside the inline storage.

{%- capture unified -%}
<span class="cpp17">[[nodiscard]]</span> <span class="cpp20">constexpr</span>
bool
inlinable (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
[[nodiscard]] constexpr
bool
inlinable (void) const noexcept;
{%- endcapture -%}

{%- capture cpp17 -%}
[[nodiscard]]
bool
inlinable (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
bool
inlinable (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp17=cpp17 cpp11=cpp11 %}
