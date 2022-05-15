---
title: insert
parent: small_vector
---

# insert

Insert elements into the container at a specified position.

{%- capture unified -%}
<span class="cpp20">constexpr</span>
iterator
insert (const_iterator pos, const_reference value)<span class="cpp20">
  requires CopyInsertable && CopyAssignable</span>;

<span class="cpp20">constexpr</span>
iterator
insert (const_iterator pos, value_type&& value)<span class="cpp20">
  requires MoveInsertable && MoveAssignable</span>;

<span class="cpp20">constexpr</span>
iterator
insert (const_iterator pos, size_type count, const_reference value)<span class="cpp20">
  requires CopyInsertable && CopyAssignable</span>;

template <[<span class="cpp20">std::input_iterator</span> | <span class="cpp11">typename</span>] InputIt&gt;<span class="cpp20">
requires EmplaceConstructible<std::iter_reference_t<InputIt&gt;&gt;
     &&  MoveInsertable
     &&  MoveAssignable
constexpr</span>
iterator
insert (const_iterator pos, InputIt first, InputIt last);

<span class="cpp20">constexpr</span>
iterator
insert (const_iterator pos, std::initializer_list<value_type&gt; ilist)<span class="cpp20">
  requires EmplaceConstructible<const_reference&gt;
       &&  MoveInsertable
       &&  MoveAssignable</span>;
{%- endcapture -%}

{%- capture cpp20 -%}
constexpr
iterator
insert (const_iterator pos, const_reference value)
  requires CopyInsertable && CopyAssignable;

constexpr
iterator
insert (const_iterator pos, value_type&& value)
  requires MoveInsertable && MoveAssignable;

constexpr
iterator
insert (const_iterator pos, size_type count, const_reference value)
  requires CopyInsertable && CopyAssignable;

template <std::input_iterator InputIt>
requires EmplaceConstructible<std::iter_reference_t<InputIt>>
     &&  MoveInsertable
     &&  MoveAssignable
constexpr
iterator
insert (const_iterator pos, InputIt first, InputIt last);

constexpr
iterator
insert (const_iterator pos, std::initializer_list<value_type> ilist)
  requires EmplaceConstructible<const_reference>
       &&  MoveInsertable
       &&  MoveAssignable;
{%- endcapture -%}

{%- capture cpp11 -%}
iterator
insert (const_iterator pos, const_reference value);

iterator
insert (const_iterator pos, value_type&& value);

iterator
insert (const_iterator pos, size_type count, const_reference value);

template <typename InputIt>
iterator
insert (const_iterator pos, InputIt first, InputIt last);

iterator
insert (const_iterator pos, std::initializer_list<value_type> ilist);
{%- endcapture -%}

{% include prototype.html unified=unified cpp20=cpp20 cpp11=cpp11 %}

## Details

{% capture parameters %}
pos | An iterator before which a copy of `value` will be inserted.
value | An object to be copied.
{% endcapture %}

{% include reference_card.html
   prototype_id="1"
   title="Insert a copy of an object"
   description="Inserts a copy of `value` immediately before `pos`."
   parameters=parameters
   return="An iterator pointing to the inserted element."
   complexity="𝓞(n), where n varies with respect to the distance between `pos` and `end()`."
   exceptions="A strong exception guarantee is provided if `pos == end()`, and `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`value_type` must be [CopyInsertable] and [CopyAssignable]."
   %}

{% capture parameters %}
pos | An iterator before which an element will be inserted.
value | An object to be moved.
{% endcapture %}

{% include reference_card.html
   prototype_id="2"
   title="Move-insert an object"
   description="Move constructs a new element immediately before `pos` using `value`."
   parameters=parameters
   return="An iterator pointing to the inserted element."
   complexity="𝓞(n), where n varies with respect to the distance between `pos` and `end()`."
   exceptions="A strong exception guarantee is provided if `pos == end()`, and `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`value_type` must be [MoveInsertable] and [MoveAssignable]."
   notes="Behavior is unspecified if `value` is located at or after `pos` inside the container."
   %}

{% capture parameters %}
pos | An iterator before which the elements will be inserted.
count | The number of elements to be inserted.
value | An object to be copied for each new element.
{% endcapture %}

{% include reference_card.html
   prototype_id="3"
   title="Insert a number of copies of an object"
   description="Inserts a number of copies of an object immediately before `pos`."
   parameters=parameters
   return="If `count == 0`, then `pos`. Otherwise, an iterator pointing to the first element that was inserted."
   complexity="𝓞(n + m), where n varies with respect to the distance between `pos` and `end()`, and m varies with respect to `count`."
   exceptions="A strong exception guarantee is provided if `pos == end()` and `count == 1`, and `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`value_type` must be [CopyInsertable] and [CopyAssignable]."
   %}

{% capture parameters %}
pos | An iterator before which the range of elements will be inserted.
first | An iterator pointing to the first element of the insertion range.
last | An iterator one past the end of the insertion range.
{% endcapture %}

{% include reference_card.html
   prototype_id="4"
   title="Insert a range of elements"
   description="Inserts a copy of each element in the range defined from `first` to `last` into the position immediately before `pos`."
   parameters=parameters
   return="If `first == last`, then `pos`. Otherwise, an iterator pointing to the first element that was inserted."
   complexity="𝓞(n + m), where n varies with respect to the distance between `pos` and `end()`, and m varies with respect to the distance between `first` and `last`."
   exceptions="A strong exception guarantee is provided if `pos == end()` and the distance between `first` and `last` is equal to 1, and `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`InputIt` must meet the requirements of [InputIterator], and `value_type` must be [MoveInsertable] and [MoveAssignable], as well as [EmplaceConstructible] with the type returned when dereferencing an object of type `InputIt`."
   %}

{% capture parameters %}
pos | An iterator before which the range of elements will be inserted.
ilist | An initializer list of elements to be inserted.
{% endcapture %}

{% include reference_card.html
   prototype_id="5"
   title="Insert an initializer list of elements"
   description="Inserts a copy of each element in the initializer list into the position immediately before `pos`."
   parameters=parameters
   return="If size of `ilist` is equal to 0, then `pos`. Otherwise, an iterator pointing to the first element that was inserted."
   complexity="𝓞(n + m), where n varies with respect to the distance between `pos` and `end()`, and m varies with respect to the size of `ilist`."
   exceptions="A strong exception guarantee is provided if `pos == end()` and the size of `ilist` is equal to 1, and `value_type` is either [CopyInsertable] or nothrow move constructible."
   requirements="`value_type` must be [MoveInsertable] and [MoveAssignable], as well as [EmplaceConstructible] with `const_reference`."
   %}

## Example

<div class="code-example" markdown="1">

{% capture ce-url %}
https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxHoADqgKhE4MHt6%2BcQlJAkEh4SxRMVy2mPaOAkIETMQEqT5%2BRXaYDskVVQQ5YZHRegqV1bXpDX3twZ353VwAlLaoXsTI7BzmAMzByN5YANQmy24IBASxCiAA9CfETADuAHTAhAheEV5K87KMBNdoLCfACFUAbpgAJ6%2BE4KFhiWgAfUBDhIJwhwTBs3mmBOaw2aOAyAQYIhtGhsKIxGuCFisR22BMGgAgisMV4tjs3E5esRMKxKdS6bTggRuYiGJsIP9UHh0JNuSYAOxWWmbTZMLxETZYFiobbLAAi2wArFZdVqIHzNrFiKgiAQgbFMFDxaRNqLxcKAFRULwMZCTYXYpAgcGQmFNYnMvlcswANkm3plcppCoVvpA/vxhODJFDgkpju2ss2RU2Zgdy1zWp2cYTm3dnuF/0lyzj3MrvXQybQys1bmZpvNluttudzO75jMIFzFm2ZjMne7/xM%2Bo087LDabCf4xGFSpVhE1OpbyZCqgIteuUTuQog0fLmx3YDAOx1/2urmF9Yn1mshEl8srCf3KFmY8h12Sci1AmcQJdQhy1XBV/3bIDdmHKdSxHCC3E2f9XFoGCfxlZdGx/NUNQgAt5wNI0kxTQMiQzXYw2WKlI0dGNZVgzY0AYXob0ETZVF3TYAFouFw%2BMEyfYIXmPCB/0PaSnzPYJX2mPi31XfC1NpVdiOFMDyKXaAcWTAMCSDOFiEzflGPMCMWPHdiJK46JpNkzAjxPRSL2jB1BLMTSxI00TtMwdVhWWB19MNQy/RMtNzMs8NbLreyfwVTjuJNfiHyE5ZRMrRypOFVz3JFU9MHPZTiwdVR/IVQKVy0oiQpI6Q9Qo6LjNTMyQ3orNrOY5LY3Yk0mEinVstjITWsE3UfNs/C8vE65JOcoqCFbEA5I88qlMvFT/08zd9vW5MXwgJgrwagLpS1fzgtCiBZragyqNi7q6JZPqmKS1iKyWlbqjWjattKw69oivNBOlHyAA4fIATlLWqkaCn92QIOYhUXK6FtpDhploThdV4PwOC0UhUE4LtLGsTCUQWSdlh4UgCE0fHpgAaxAXUNH0ThJF4FgJA0XnSfJymOF4Y5edZsn8dIOBYBgRAAJYWI6GichKC%2BdX6BiYAuC4IsaFoAhomOCAIjZ0gImCKogU4ZmvjYQQAHkGFoB25dINVDGAcRvfwdlmkBY5vbcpplUWcm%2BRKa3aDwCILmIIEPCwa2CGIPAhe4eWqAMYAFAANTwTBLldm1SeZ/hBBEMR2CkGRBEUFR1G93QigMIwUA/Sx9ET45IGmVBYjKLjOClkpg2SFwGHcTw6n8OeOjyAoMkSMeBnqUh4g35IV66QpilKFphi3nop%2BacphgP8Yj96Npz6GNpb7XqYZjmBY9EzzBFh4AmiYk2thLVQMMIyCQjJITY2JkD5i4NcacEBqZWD7psXAhASCMwLB4NWGsNwrCmLwWWWhvIIA5FgGIl5SBcx5nzDgAtSBC1oWLXgEspYgBlmzaYisVaAViMqLWEAdZ4P8PgYkega7CFEOIRukiW5qAzuyP%2BvBLgXFiI7eWhMODE1ICwimnBXbKn4ceVAVA%2BJgIgVAmBcCEHChwbraIWDJhEK4dMMhTAKGUAAfQwW3NRbAInrYDhLNXHUL8XQ5YQDvZsJCXLbyWizBRPFoE4h7NSCAmIIkZwkggA%3D%3D%3D
{% endcapture %}

{% include compiler_explorer_link.html href=ce-url %}

<h3>Code</h3>

```c++
#include <gch/small_vector.hpp>
#include <iostream>
 
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
    const int x = -1;
    v.insert (std::next (v.begin ()), x);
  });

  demo (2, [](gch::small_vector<int>& v) {
    v.insert (std::next (v.begin ()), -2);
  });

  demo (3, [](gch::small_vector<int>& v) {
    const int x = -3;
    v.insert (std::next (v.begin ()), 3, x);
  });

  demo (4, [](gch::small_vector<int>& v) {
    int a[] = { -4, -5, -6 };
    v.insert (std::next (v.begin ()), std::begin (a), std::end (a));
  });

  demo (5, [](gch::small_vector<int>& v) {
    v.insert (std::next (v.begin ()), { -7, -8, -9 });
  });

  return 0;
}
```

<h3>Output</h3>

```text
1: { 1, -1, 2, 3 }
2: { 1, -2, 2, 3 }
3: { 1, -3, -3, -3, 2, 3 }
4: { 1, -4, -5, -6, 2, 3 }
5: { 1, -7, -8, -9, 2, 3 }
```

</div>

{% include common_links.md %}
