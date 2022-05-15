---
title: append
parent: small_vector
---

# append

Append a range of elements to the end of the container.

{%- capture unified -%}
template <[<span class="cpp20">std::input_iterator</span> | <span class="cpp11">typename</span>] InputIt&gt;<span class="cpp20">
requires EmplaceConstructible<std::iter_reference_t<InputIt&gt;&gt;
     &&  MoveInsertable
constexpr</span>
small_vector&
append (InputIt first, InputIt last);

<span class="cpp20">constexpr</span>
small_vector&
append (std::initializer_list<value_type&gt; ilist)<span class="cpp20">
  requires EmplaceConstructible<const_reference&gt;
       &&  MoveInsertable</span>;

template <unsigned I&gt;
<span class="cpp20">constexpr</span>
small_vector&
append (const small_vector<T, I, Allocator&gt;& other)<span class="cpp20">
  requires CopyInsertable</span>;

template <unsigned I&gt;
<span class="cpp20">constexpr</span>
small_vector&
append (small_vector<T, I, Allocator&gt;&& other)<span class="cpp20">
  requires MoveInsertable</span>;
{%- endcapture -%}

{%- capture cpp20 -%}
template <std::input_iterator InputIt>
requires EmplaceConstructible<std::iter_reference_t<InputIt>>
     &&  MoveInsertable
constexpr
small_vector&
append (InputIt first, InputIt last);

constexpr
small_vector&
append (std::initializer_list<value_type> ilist)
  requires EmplaceConstructible<const_reference>
       &&  MoveInsertable;

template <unsigned I>
constexpr
small_vector&
append (const small_vector<T, I, Allocator>& other)
  requires CopyInsertable;

template <unsigned I>
constexpr
small_vector&
append (small_vector<T, I, Allocator>&& other)
  requires MoveInsertable;
{%- endcapture -%}

{%- capture cpp11 -%}
template <typename InputIt>
small_vector&
append (InputIt first, InputIt last);

small_vector&
append (std::initializer_list<value_type> ilist);

template <unsigned I>
small_vector&
append (const small_vector<T, I, Allocator>& other);

template <unsigned I>
small_vector&
append (small_vector<T, I, Allocator>&& other);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}

## Details

{% capture parameters %}
first | An iterator pointing to the first element of the range to append.
last | An iterator one past the end of the range to append.
{% endcapture %}

{% include reference_card.html
   prototype_id="1"
   title="Append a range of elements"
   description="Appends a range of elements to the end of the container."
   parameters=parameters
   return="A reference to the object."
   complexity="𝓞(n), where n varies with respect to the distance between `first` and `last`."
   exceptions="A strong exception guarantee is provided if `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`InputIt` must meet the requirements of [InputIterator], and `value_type` must be [MoveInsertable] and [MoveAssignable], as well as [EmplaceConstructible] with the type returned when dereferencing an object of type `InputIt`."
   %}

{% capture parameters %}
ilist | An initializer list containing elements to append.
{% endcapture %}

{% include reference_card.html
   prototype_id="2"
   title="Append an initializer list of elements"
   description="Appends a copy of each element in the initializer list to the end of the container."
   parameters=parameters
   return="A reference to the object."
   complexity="𝓞(n), where n varies with respect to the size of the initializer list."
   exceptions="A strong exception guarantee is provided if `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`value_type` must be [MoveInsertable], as well as [EmplaceConstructible] with `const_reference`."
   %}

{% capture parameters %}
other | A const reference to another `small_vector`.
{% endcapture %}

{% include reference_card.html
   prototype_id="3"
   title="Append copies of elements in another container"
   description="Appends a copy of each element in the other `small_vector` to the end of the container."
   parameters=parameters
   return="A reference to the object."
   complexity="𝓞(n), where n varies with respect to the size of the other container."
   exceptions="A strong exception guarantee is provided if `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`value_type` must be [CopyInsertable]."
   %}

{% capture parameters %}
other | An rvalue reference to another `small_vector`.
{% endcapture %}

{% include reference_card.html
   prototype_id="4"
   title="Append the moved elements of another container"
   description="Moves each element from another `small_vector` to the end of the container."
   parameters=parameters
   return="A reference to the object."
   complexity="𝓞(n), where n varies with respect to the size of the other container."
   exceptions="A strong exception guarantee is provided if `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`value_type` must be [MoveInsertable]."
   %}

## Example

<div class="code-example" markdown="1">

{% capture ce-url %}
https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxHoADqgKhE4MHt6%2BcQlJAkEh4SxRMVy2mPaOAkIETMQEqT5%2BRXaYDskVVQQ5YZHRegqV1bXpDX3twZ353VwAlLaoXsTI7BzmAMzByN5YANQmy24IBASxCiAA9CfETADuAHTAhAheEV5K87KMBNdoLCfACFUAbpgAJ6%2BE4KFhiWgAfUBDhIJwhwTBs3mmBOaw2aOAyAQYIhtGhsKIxGuCFisR22BMGgAgisMV4tjs3E5esRMKxKdS6WZVgx1ozMNtdrQ8L0ubTtrTggQpTTEQxNhB/qg8OhJtyTAB2KySzZMLxETZYFioYUAEW2AFYrFbzRAZZtYsRUEQCEDYpgoWrSJsVWqlQAqKhefmTJXYpAgcGQmFNYnMmVcswANkm4e1uppmxzm0jIGj%2BMJ8ZIicElL92x1myKmzMvuWVfNOyzuc2If5Sv%2BGuWWe5bd66ALaENwrczKdLrdHq9AeZE/MZhAVYs2zMZjHE/%2BJhtGh3zd7/dz/GISoNRsIFs2g4LIVUsuV1yid0VEHTLc2l7AYB2lv%2B11cJUe1XaxrEIDU9TbHMbxQWZZXnXY13rJDN0QwNCBbI9oIIIdYNHBC3BQ7Vm3XVDCJg1xaEwvViOouk9RNM0IFrHdbXtfNC1jIlS12JNlipVM/QzHUsOvHCC1FcVePLfjNloFdNgAWlrRSN1ow9IL9a4mHJQCIFoJ9MBfIDfQMvT3w07Mm2AzUGMwU0lWQ1j92gHECxjAk4zhYgywIZMUyEldRP/HTPQYdAlUzJTll9RTJGsuic2ImzaSPRilRi602NcqMPOLbzfP8wLM1Eji8q8hNpL82TLgUxSrVigL1NbXMQt08KlUuFKrOSui0vspjpCylyyqLCqeJZGSBIC7sgs00auJLHyqorWqosUrVYoADibRLWu09qIogGDTUBTqLJa3rLKPdkCDmRU92urVm1pDhploTgrV4PwOC0UhUE4cdLGsa8UQWNdlh4UgCE0N7pgAaxAK0NH0ThJF4FgJA0FGfr%2BgGOF4Y4UZh363tIOBYBgRBYJYWI6GichKC%2BOn6BiYAuC4esaFoAhomOCAIlh0gImCKogU4KGvjYQQAHkGFocXSdIE1DGAcQlfwdlmkBY4lcwVQmkNRY/plEohdFCILmIIEPCwIWCGIPBMe4MmqAMYAFAANTwTBLhlsKJd4fhBBEMR2CkGRBEUFR1CV3QigMIwUFAyx9DwCJjkgaZUFiMoGF1wmSnjZIXHCgZ6lIQJRjyAoMkSPPy7rrIGA6GuJmKUoWmGRuGiL5pymGVuukKWxu88OoekH6vh4kaYFDB8PofZRYeHez7vqF/HVC2lNFJTeLsWQGsuGuDcICBqxU82XBCBICHaw8Wn6dPFYpl4EmtHTUgEA5LAYjfUgiNkaow4OjUgmNgG414PjQmIBiaw2mBTamcFYiGkZhATA%2BBiRqj0MHYQohxARzwdHNQQt46kEuBcWIgc14cC%2BqQKB/1OAy0NKg2UqAqCbG3rvfeeZkBHw5qfJUj8WbRHvpMd%2BCDpg/yYH/SgtCwEQJxpvTgsD4Gky/kAlGH0ODLA3krGB0MpEgLMPovGqijEaOmICYgiRnCSCAA%3D%3D%3D
{% endcapture %}

{% include compiler_explorer_link.html href=ce-url %}

<h3>Code</h3>

```c++
#include <gch/small_vector.hpp>
#include <iostream>
#include <list>
 
int 
main (void)
{
  auto demo = [](int prototype_id, void (*func) (gch::small_vector<int>&)) {
    gch::small_vector<int> v { 1, 2, 3 };
    func (v);

    std::cout << prototype_id << ": { " << v[0];
    for (auto it = std::next (v.begin ()); it != v.end (); ++it)
        std::cout << ", " << *it;
    std::cout << " }" << std::endl;
  };

  demo (1, [](gch::small_vector<int>& v) {
    std::list<int> l { -1, -2 };
    v.append (l.begin (), l.end ());
  });

  demo (2, [](gch::small_vector<int>& v) {
    v.append ({ -3, -4 });
  });

  demo (3, [](gch::small_vector<int>& v) {
    gch::small_vector<int> w { -5, -6 };
    v.append (w);
  });

  demo (4, [](gch::small_vector<int>& v) {
    gch::small_vector<int> w { -7, -8 };
    v.append (std::move (w));
  });

  return 0;
}
```

<h3>Output</h3>

```text
1: { 1, 2, 3, -1, -2 }
2: { 1, 2, 3, -3, -4 }
3: { 1, 2, 3, -5, -6 }
4: { 1, 2, 3, -7, -8 }
```

</div>

{% include common_links.md %}
