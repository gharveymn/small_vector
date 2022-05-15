---
title: get_allocator
parent: small_vector
---

# get_allocator

Retrieve a copy of the allocator used by the container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
allocator_type
get_allocator (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
allocator_type
get_allocator (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
allocator_type
get_allocator (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
