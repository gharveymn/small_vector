---
title: max_size
parent: small_vector
---

# max_size

Retrieve the maximum number of elements that may be held by this container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
size_type
max_size (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
size_type
max_size (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
size_type
max_size (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
