---
title: cbegin
parent: small_vector
---

# cbegin

Retrieve a const iterator pointing to the first element in the container.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
const_iterator
cbegin (void) const noexcept;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
const_iterator
cbegin (void) const noexcept;
{%- endcapture -%}

{%- capture cpp11 -%}
const_iterator
cbegin (void) const noexcept;
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}

## Parameters

None.

## Return

An const iterator pointing to the first element in the container.

## Complexity

𝓞(1).

## Exceptions

This function does not throw exceptions.

## Notes

When the container is empty, the value returned by this function is equal to
that of [end()](end).

## Example

<div class="code-example" markdown="1">

{% capture ce-url %}
https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxHoADqgKhE4MHt6%2BcQlJAkEh4SxRMVy2mPaOAkIETMQEqT5%2BRXaYDskVVQQ5YZHRegqV1bXpDX3twZ353VwAlLaoXsTI7BzmAMzByN5YANQmy24IBASxCiAA9CfETADuAHTAhAheEV5K87KMBNdoLCfACFUAbpgAJ6%2BE4KFhiWgAfUBDhIJwhwTBs3mmBOaw2aOAyAQYIhtGhsKIxGuCFisR22BMGgAgisMV4tjs3E5esRMKxKdSadtacECLyaYiGJsIP9UHh0JNuSYAOxWWmbTbYpAgcGQmFNYnM/mUzb/HYKnmbNAMXrKnEgNX4wlakg6wR6y6G7lK3pMRzIKFMBQvAUQXroK14BRQhSsTDMrDrAhA2KYUX/T5RO4iiCTaabaO0WPx0U3FPBUUZylW/5iLyYUiCpW1uv12vmMyhVAC029KGEaIekjXJvS5ZG12bQNWtBeAXM5nbMxmZCFhjp7bLAAiOxXJtc6Z2ADEm8u3NPRyBXLQXYqRwQgyhZpPdtP95sAOoIRibTAsWKxkAzswH6dismmCpsWy5rqu%2BrXK4xaTP%2BuyXtep7nnSF5JrEzwIFCERMMgADWooALRTMhbpXmOt5wW4v5Ki%2Bb4MAIBEfl%2BQI/vuU7wYB87AUWS7ruukHQemsHsVRx5IYOw7HuOd6HvBTYAFRcam6asbOlGbPJSZKTxwn3vBYkMOgZ4SbSw7sgQcwihoyFymutIcNMtCcAArLwfgcFopCoJwh6WNYI4ogsM7LDwpAEJoDnTLhIDORo%2BicJIvAsBIGhxe5nneRwvDHHF4UeQ5pBwLAMCIDen50NE5CUF8sQVTEwBcFwZh8HQBDRMcEARBFpARMEVQsdwvBfGwggAPIMLQA2eVgEJGOI%2BWkPg7LNICxwLZgqhNBOiyefyJTdbQeARBcxBAh4WDdQQxB4Mlg3TFQBjAAoABqeCYJco3xu5oX8IIIhiOwUgyIIigqOoC26EUBhGCg1jWPoR3HJA0yoF%2ByRrdlJRaskLiGQM9SkIEox5AUGSJGUKSeHUZNZAwHQkxMxSlC0wz4z0WPNOUwz010hS2KzVODPzbQ8%2BMhTTAogWA2F7KLDwjkuW53WZaoAAcABsBHq5IFrIJsjXXH%2BEC%2BVYljVrghAkMFRSbB45X0MQ1uTLweVaBmpCvkwWAxOmpDRbF8UcIlpDJQH6W8Jl2UgLlEXTEVpW3uhBBVRANV1f4%2BDEnov3CKI4hAznoNqJdsucKFlwXLEZcFU5HCuaQ4deZwo0TknmyoFQmxq5r2u6/rXCG6Kdu1Q7Tsu7H0ye97lAK0HSUxWlyucFHMf5e7/txbXyxKwtkdhRPgdmDvGXL/va/TICxCJM4khAA%3D
{% endcapture %}

{% include compiler_explorer_link.html href=ce-url %}

<h3>Code</h3>

```c++
#include <gch/small_vector.hpp>
#include <iostream>
 
int 
main (void)
{
  gch::small_vector<int> v;
  const gch::small_vector<int> w;
  static_assert (std::is_same<decltype (v.cbegin ()), decltype (w.begin ())>::value, 
                 "Not const_iterator.");

  std::cout << "cbegin() == cend()?" << std::endl;
  std::cout << "  When empty: " << (v.cbegin () == v.end ()) << std::endl;

  v.push_back (-1);
  std::cout << "  When non-empty: " << (v.cbegin () == v.end ()) << std::endl;
  std::cout << "*cbegin(): " << *v.cbegin () << std::endl;

  return 0;
}
```

<h3>Output</h3>

```text
begin() == end()?
  When empty: 1
  When non-empty: 0
*begin(): -1
```

</div>

## See Also

- [gch::cbegin()](non-member/cbegin)
- [begin()](begin)
- [cend()](cend)
