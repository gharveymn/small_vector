---
title: empty
parent: Non-Member Functions
grand_parent: small_vector
---

# gch::empty

Query whether the container is empty.

{%- capture unified -%}
template <typename T, unsigned InlineCapacity, typename Allocator&gt;
<span class="cpp17">[[nodiscard]]</span> <span class="cpp20">constexpr</span>
bool
empty (const small_vector<T, InlineCapacity, Allocator&gt;& v) noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
[[nodiscard]] constexpr
bool
empty (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{%- capture cpp17 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
[[nodiscard]]
bool
empty (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename T, unsigned InlineCapacity, typename Allocator>
bool
empty (const small_vector<T, InlineCapacity, Allocator>& v) noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp17=cpp17 cpp11=cpp11 %}
