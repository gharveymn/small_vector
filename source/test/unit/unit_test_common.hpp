/** unit_test_common.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SMALL_VECTOR_UNIT_TEST_COMMON_HPP
#define SMALL_VECTOR_UNIT_TEST_COMMON_HPP

#include "test_common.hpp"

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
#  include GCH_SMALL_VECTOR_TEST_FILE
#else
extern template class gch::small_vector<int>;

int
test (void);
#endif

#endif // SMALL_VECTOR_UNIT_TEST_COMMON_HPP
