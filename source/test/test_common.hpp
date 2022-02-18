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

#include "test_types.hpp"

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR constexpr
#else
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR
#endif

#if defined (GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR)
#  define CHECK(EXPR) \
if (! (EXPR))         \
  return 1
#elif defined (GCH_LIB_IS_CONSTANT_EVALUATED)
#  define CHECK(EXPR)                                                                              \
if (! (EXPR))                                                                                      \
{                                                                                                  \
  if (! std::is_constant_evaluated ())                                                             \
  {                                                                                                \
    std::fprintf (stderr, "Check failed in file " __FILE__ " at line %i:\n" #EXPR "\n", __LINE__); \
    std::fflush (stderr);                                                                          \
  }                                                                                                \
  return 1;                                                                                        \
} (void)0
#else
#  define CHECK(EXPR)                                                                            \
if (! (EXPR))                                                                                    \
{                                                                                                \
  std::fprintf (stderr, "Check failed in file " __FILE__ " at line %i:\n" #EXPR "\n", __LINE__); \
  std::fflush (stderr);                                                                          \
  return 1;                                                                                      \
} (void)0
#endif

#endif // OCTAVE_IR_TEST_COMMON_HPP
