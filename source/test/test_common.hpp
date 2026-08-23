/** test_common.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SMALL_VECTOR_TEST_COMMON_HPP
#define SMALL_VECTOR_TEST_COMMON_HPP

#ifdef GCH_SMALL_VECTOR_TEST_REL_OPS_AMBIGUITY
#  include <utility>

#  ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wheader-hygiene"
#  endif

using namespace std::rel_ops;

#  ifdef __clang__
#    pragma clang diagnostic pop

#    include <memory>

template <typename T>
constexpr
bool
operator!= (const std::allocator<T>&, const std::allocator<T>&) noexcept
{
  return false;
}

#  endif
#endif

#include "gch/small_vector.hpp"

#define CHECK_(...)                                                                       \
(void)(                                                                                   \
    !! (__VA_ARGS__)                                                                      \
  ||   ( std::fprintf (                                                                   \
           stderr,                                                                        \
           "Check failed at " __FILE__ " at line %i for expression:\n" #__VA_ARGS__ "\n", \
           __LINE__                                                                       \
         ),                                                                               \
         std::abort (),                                                                   \
         0                                                                                \
       )                                                                                  \
)

#ifdef GCH_LIB_IS_CONSTANT_EVALUATED
#  define CHECK(...)                                                                      \
(void)((std::is_constant_evaluated () ? assert ((__VA_ARGS__)) : CHECK_(__VA_ARGS__)), 0)
#else
#  define CHECK(...) CHECK_(__VA_ARGS__)
#endif

#ifdef GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR constexpr
#  define CHECK_IF_NOT_CONSTEXPR(...)
#else
#  define GCH_SMALL_VECTOR_TEST_CONSTEXPR
#  define CHECK_IF_NOT_CONSTEXPR(...) CHECK (__VA_ARGS__)
#  if ! defined (GCH_SMALL_VECTOR_TEST_DISABLE_EXCEPTION_SAFETY_TESTING) && defined (GCH_EXCEPTIONS)
#    define GCH_SMALL_VECTOR_TEST_EXCEPTION_SAFETY_TESTING
#  endif
#endif

#ifdef GCH_SMALL_VECTOR_TEST_EXCEPTION_SAFETY_TESTING
#  ifdef GCH_LIB_IS_CONSTANT_EVALUATED
#    define EXPECT_THROW(...)                                                                     \
if (! std::is_constant_evaluated ())                                                              \
{                                                                                                 \
  __VA_ARGS__;                                                                                    \
  std::fprintf (                                                                                  \
    stderr,                                                                                       \
    "Missing expected throw in file " __FILE__ " at line %i for expression:\n" #__VA_ARGS__ "\n", \
    __LINE__                                                                                      \
  );                                                                                              \
  std::abort ();                                                                                  \
} (void)0
#  else
#    define EXPECT_THROW(...)                                                                     \
do                                                                                                \
{                                                                                                 \
  __VA_ARGS__;                                                                                    \
  std::fprintf (                                                                                  \
    stderr,                                                                                       \
    "Missing expected throw in file " __FILE__ " at line %i for expression:\n" #__VA_ARGS__ "\n", \
    __LINE__                                                                                      \
  );                                                                                              \
  std::abort ();                                                                                  \
} while (0)
#  endif
#else
#  define EXPECT_THROW(...)
#endif

#endif // SMALL_VECTOR_TEST_COMMON_HPP
