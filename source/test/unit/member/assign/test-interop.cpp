/** test-interop.cpp
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"

#include <array>
#include <valarray>
#include <vector>

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  std::vector<int> src_vec { 1, 2, 3, 4 };
  gch::small_vector<int, 2> from_vec (src_vec.begin (), src_vec.end ());
  CHECK (from_vec.size () == src_vec.size ());
  CHECK (std::equal (src_vec.begin (), src_vec.end (), from_vec.begin ()));

  std::array<int, 4> src_arr { 5, 6, 7, 8 };
  gch::small_vector<int, 2> from_arr;
  from_arr.assign (src_arr.begin (), src_arr.end ());
  CHECK (from_arr.size () == src_arr.size ());
  CHECK (std::equal (src_arr.begin (), src_arr.end (), from_arr.begin ()));

  const std::valarray<int> src_val { 9, 10, 11, 12 };
  gch::small_vector<int, 2> from_val;
  from_val.assign (std::begin (src_val), std::end (src_val));
  CHECK (from_val.size () == src_val.size ());
  CHECK (std::equal (std::begin (src_val), std::end (src_val), from_val.begin ()));

  return 0;
}
