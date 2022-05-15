---
title: inline_capacity
parent: small_vector
---

# inline_capacity

Retrieve the number of elements which may fit in the inline storage associated with this container.

{%- capture unified -%}
<span class="cpp17">[[nodiscard]]</span>
static [<span class="cpp20">consteval</span> | <span class="cpp11">constexpr</span>]
size_type
inline_capacity (void) noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
[[nodiscard]]
static consteval
size_type
inline_capacity (void) noexcept;
{%- endcapture -%}

{%- capture cpp17 -%}
[[nodiscard]]
static constexpr
size_type
inline_capacity (void) noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
static constexpr
size_type
inline_capacity (void) noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp17=cpp17 cpp11=cpp11 %}
