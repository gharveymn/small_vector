/** unit_test_common.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SMALL_VECTOR_UNIT_TEST_COMMON_HPP
#define SMALL_VECTOR_UNIT_TEST_COMMON_HPP

#include "test_common.hpp"
#include "test_types.hpp"

extern template class gch::small_vector<int>;

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void);

#ifdef GCH_SMALL_VECTOR_TEST_FILE
#  define QUOTED_HELPER(...) #__VA_ARGS__
#  define QUOTED(...) QUOTED_HELPER (__VA_ARGS__)
#  include QUOTED (GCH_SMALL_VECTOR_TEST_FILE)
#endif

#endif // SMALL_VECTOR_UNIT_TEST_COMMON_HPP
