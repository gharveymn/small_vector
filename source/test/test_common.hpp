/** test_common.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SMALL_VECTOR_TEST_COMMON_HPP
#define SMALL_VECTOR_TEST_COMMON_HPP

// This is for testing for ambiguity in comparison operators.
// #ifdef _MSC_VER
// #  define _SILENCE_CXX20_REL_OPS_DEPRECATION_WARNING 1
// #endif
//
// #include <utility>
// using namespace std::rel_ops;

#include "gch/small_vector.hpp"

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR constexpr
#else
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR
#endif

#define CHECK(...) assert ((__VA_ARGS__))

#if defined (GCH_LIB_IS_CONSTANT_EVALUATED)

#  define EXPECT_THROW(...)                                                                       \
if (! std::is_constant_evaluated ())                                                              \
{                                                                                                 \
  __VA_ARGS__;                                                                                    \
  std::fprintf (                                                                                  \
    stderr,                                                                                       \
    "Missing expected throw in file " __FILE__ " at line %i for expression:\n" #__VA_ARGS__ "\n", \
    __LINE__);                                                                                    \
  return 1;                                                                                       \
} (void)0

#else

#  define EXPECT_THROW(...)                                                                     \
__VA_ARGS__;                                                                                    \
std::fprintf (                                                                                  \
  stderr,                                                                                       \
  "Missing expected throw in file " __FILE__ " at line %i for expression:\n" #__VA_ARGS__ "\n", \
  __LINE__);                                                                                    \
return 1

#endif



#endif // SMALL_VECTOR_TEST_COMMON_HPP
