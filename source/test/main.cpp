/** main.cpp
 * Main testing
 *
 * Copyright © 2021 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "gch/small_vector.hpp"

#include "test-types.hpp"

#include <iostream>

template class gch::small_vector<int>;
template class gch::small_vector<std::size_t>;
template class gch::small_vector<double>;
template class gch::small_vector<char *>;

template class gch::small_vector<double, 8, gch::test_types::weird_allocator1<double>>;
template class gch::small_vector<double, 8, gch::test_types::weird_allocator2<double>>;

int main (void)
{
  using namespace gch;
  small_vector<int> v { 1, 2, 3 };
  v.insert (v.begin () + 1, 7);
  for (auto e : v)
    std::cout << e << std::endl;

  small_vector<test_types::uncopyable> u { };
  small_vector<test_types::uncopyable> uu { };

  return 0;
}
