/** main.cpp.c
 * Short description here.
 *
 * Copyright © 2021 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "gch/small_vector.hpp"
#include <numeric>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

int accum (std::initializer_list<int> ii)
{
  gch::small_vector<int> v (ii);
  return std::accumulate (v.begin (), v.end (), 0);
}

TEST_CASE ("test accum")
{
  CHECK (accum ({  1,  2,  3 }) ==  6);
  CHECK (accum ({  4,  2,  3 }) ==  9);
  CHECK (accum ({  2, -2, -3 }) == -3);
  CHECK (accum ({  1,  2, -7 }) == -4);
}
