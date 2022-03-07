/** test-ilist.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

static GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_exceptions (void)
{
  using namespace gch;

  using test_type = test_types::triggering_copy_ctor;

  std::initializer_list<test_type> values { 1, 2, 3, 4, 5 };
  small_vector_with_allocator<test_type, test_types::sized_allocator<test_type, std::uint8_t>> v;

  // Append an empty range.
  v.append (values.begin (), values.begin ());
  v.append (values.end (), values.end ());

  CHECK (v.empty ());

  v.append (values.begin (), values.end ());

  CHECK (v.size () == values.size ());
  CHECK (std::equal (v.begin (), v.end (), values.begin ()));

  CHECK (std::numeric_limits<std::uint8_t>::max () / sizeof (test_type) == v.max_size ());
  v.reserve (v.max_size ());

  const auto save = v;

  // Throw at the very beginning.
  test_types::global_exception_trigger ().push (0);
  try
  {
    EXPECT_THROW (v.append (values.begin (), values.end ()));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (v == save);
  }

  // Throw in the middle.
  test_types::global_exception_trigger ().push (values.size () / 2);
  try
  {
    EXPECT_THROW (v.append (values.begin (), values.end ()));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (v == save);
  }

  // Throw at the end.
  test_types::global_exception_trigger ().push (values.size () - 1);
  try
  {
    EXPECT_THROW (v.append (values.begin (), values.end ()));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (v == save);
  }

  v.clear ();
  v.shrink_to_fit ();

  for (std::size_t i = 0; i < v.max_size () / values.size (); ++i)
    v.append (values.begin (), values.end ());

  const auto length_check_save = v;

  try
  {
    EXPECT_THROW (v.append (values.begin (), values.end ()));
  }
  catch (const std::length_error&)
  {
    CHECK (v == length_check_save);
  }

  return 0;
}

static GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_conversion (void)
{
  using namespace gch;

  using test_type = test_types::triggering_copy_ctor;

  std::initializer_list<int> values { 1, 2, 3, 4, 5 };

  small_vector_with_allocator<test_type, test_types::sized_allocator<test_type, std::uint8_t>> v;

  // Append an empty range.
  v.append (values.begin (), values.begin ());
  v.append (values.end (), values.end ());

  CHECK (v.empty ());

  v.append (values.begin (), values.end ());

  CHECK (v.size () == values.size ());
  CHECK (std::equal (v.begin (), v.end (), values.begin ()));

  CHECK (std::numeric_limits<std::uint8_t>::max () / sizeof (test_type) == v.max_size ());
  v.reserve (v.max_size ());

  v.clear ();
  v.shrink_to_fit ();

  for (std::size_t i = 0; i < v.max_size () / values.size (); ++i)
    v.append (values.begin (), values.end ());

  const auto length_check_save = v;

  try
  {
    EXPECT_THROW (v.append (values.begin (), values.end ()));
  }
  catch (const std::length_error&)
  {
    CHECK (v == length_check_save);
  }

  return 0;
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  CHECK (0 == test_exceptions ());
  CHECK (0 == test_conversion ());
  return 0;
}
