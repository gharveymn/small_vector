---
title: small_vector_iterator
parent: small_vector_iterator
---

# Constructor
{: .body-font }

Construct a `small_vector_iterator`.

{% capture unified %}
constexpr explicit
small_vector_iterator (const Pointer& p) noexcept;

template <typename U, typename D>
constexpr <span class="cpp20">explicit (false)</span>
small_vector_iterator (const small_vector_iterator<U, D>& other) noexcept;
{% endcapture %}

{% capture cpp20 %}

template <typename U, typename D>
constexpr explicit (false)
small_vector_iterator (const small_vector_iterator<U, D>& other) noexcept;
{% endcapture %}

{% capture cpp11 %}
constexpr explicit
small_vector_iterator (const Pointer& p) noexcept;

template <typename U, typename D>
constexpr
small_vector_iterator (const small_vector_iterator<U, D>& other) noexcept;
{% endcapture %}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}
