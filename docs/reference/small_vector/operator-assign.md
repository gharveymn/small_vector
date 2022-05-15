---
title: operator=
parent: small_vector
---

# operator=

Assign a value to the container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
small_vector&
operator= (const small_vector& other)<span class="cpp20">
  requires CopyInsertable && CopyAssignable</span>;

<span class="cpp20">constexpr</span>
small_vector&
operator= (small_vector&& other)
  noexcept (/* See below. */)<span class="cpp20">
  requires MoveInsertable && MoveAssignable</span>;

<span class="cpp20">constexpr</span>
small_vector&
operator= (std::initializer_list<value_type&gt; ilist)<span class="cpp20">
  requires CopyInsertable && CopyAssignable</span>;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
small_vector&
operator= (const small_vector& other)
  requires CopyInsertable && CopyAssignable;

constexpr
small_vector&
operator= (small_vector&& other)
  noexcept (/* See below. */)
  requires MoveInsertable && MoveAssignable;

constexpr
small_vector&
operator= (std::initializer_list<value_type> ilist)
  requires CopyInsertable && CopyAssignable;
{%- endcapture -%}

{%- capture cpp11 -%}
small_vector&
operator= (const small_vector& other);

small_vector&
operator= (small_vector&& other)
  noexcept (/* See below. */);

small_vector&
operator= (std::initializer_list<value_type> ilist);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
