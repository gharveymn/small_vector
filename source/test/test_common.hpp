/** test_common.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef OCTAVE_IR_TEST_COMMON_HPP
#define OCTAVE_IR_TEST_COMMON_HPP

#ifdef _MSC_VER
#  define _SILENCE_CXX20_REL_OPS_DEPRECATION_WARNING 1
#endif

#include <utility>

// This is for testing for ambiguity in comparison operators.
// using namespace std::rel_ops;

#include "gch/small_vector.hpp"

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR constexpr
#else
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR
#endif

#if defined (GCH_LIB_IS_CONSTANT_EVALUATED)

#  define CHECK(...)                                                       \
if (! (__VA_ARGS__))                                                       \
{                                                                          \
  if (! std::is_constant_evaluated ())                                     \
  {                                                                        \
    std::fprintf (                                                         \
      stderr,                                                              \
      "Check failed in file " __FILE__ " at line %i:\n" #__VA_ARGS__ "\n", \
      __LINE__);                                                           \
    std::fflush (stderr);                                                  \
  }                                                                        \
  return 1;                                                                \
} (void)0

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

#  define CHECK(...)                                                     \
if (! (__VA_ARGS__))                                                     \
{                                                                        \
  std::fprintf (                                                         \
    stderr,                                                              \
    "Check failed in file " __FILE__ " at line %i:\n" #__VA_ARGS__ "\n", \
    __LINE__);                                                           \
  std::fflush (stderr);                                                  \
  return 1;                                                              \
} (void)0

#  define EXPECT_THROW(...)                                                                     \
__VA_ARGS__;                                                                                    \
std::fprintf (                                                                                  \
  stderr,                                                                                       \
  "Missing expected throw in file " __FILE__ " at line %i for expression:\n" #__VA_ARGS__ "\n", \
  __LINE__);                                                                                    \
return 1

#endif



#endif // OCTAVE_IR_TEST_COMMON_HPP
