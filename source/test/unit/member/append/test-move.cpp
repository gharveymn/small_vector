/** test-move.cpp
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

  // This type simulates the situation when we have a throwing move constructor with a defined
  // copy constructor. The strong exception guarantees will make it so that we use the copy
  // constructor when invoking `append`.
  using test_type = test_types::triggering_ctor;

  using alloc_type = test_types::sized_allocator<test_type, std::uint8_t>;

  small_vector<test_type, 5, alloc_type> orig_values { 1, 2, 3, 4, 5 };

  small_vector<test_type, 5, alloc_type> values;
  small_vector<test_type, 6, alloc_type> v;

  // Append an empty range.
  v.append (std::move (values));

  CHECK (v.empty ());

  values = orig_values;
  v.append (std::move (values));

  CHECK (values.empty ());

  values = orig_values;
  CHECK (v.size () == values.size ());
  CHECK (std::equal (v.begin (), v.end (), values.begin ()));

  CHECK (std::numeric_limits<std::uint8_t>::max () / sizeof (test_type) == v.max_size ());
  v.reserve (v.max_size ());

  const auto save = v;

  // Throw at the very beginning.
  test_types::global_exception_trigger.reset (1);
  try
  {
    EXPECT_THROW (v.append (std::move (values)));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (values == orig_values);
    CHECK (v == save);
  }

  // Throw in the middle.
  values = orig_values;
  test_types::global_exception_trigger.reset (values.size () / 2);
  try
  {
    EXPECT_THROW (v.append (std::move (values)));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (values == orig_values);
    CHECK (v == save);
  }

  // Throw at the end.
  values = orig_values;
  test_types::global_exception_trigger.reset (values.size ());
  try
  {
    EXPECT_THROW (v.append (std::move (values)));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (values == orig_values);
    CHECK (v == save);
  }

  v.clear ();
  v.shrink_to_fit ();

  for (std::size_t i = 0; i < v.max_size () / orig_values.size (); ++i)
  {
    values = orig_values;
    v.append (std::move (values));
  }

  const auto length_check_save = v;

  values = orig_values;
  try
  {
    EXPECT_THROW (v.append (std::move (values)));
  }
  catch (const std::length_error&)
  {
    CHECK (values == orig_values);
    CHECK (v == length_check_save);
  }

  return 0;
}

static GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_normal (void)
{
  using namespace gch;

  using test_type = int;
  using alloc_type = test_types::sized_allocator<test_type, std::uint8_t>;

  small_vector<test_type, 5, alloc_type> orig_values { 1, 2, 3, 4, 5 };

  small_vector<test_type, 5, alloc_type> values;
  small_vector<test_type, 6, alloc_type> v;

  // Append an empty range.
  v.append (std::move (values));

  CHECK (v.empty ());

  values = orig_values;
  v.append (std::move (values));

  CHECK (values.empty ());

  values = orig_values;
  CHECK (v.size () == values.size ());
  CHECK (std::equal (v.begin (), v.end (), values.begin ()));

  v.clear ();
  v.shrink_to_fit ();

  for (std::size_t i = 0; i < v.max_size () / orig_values.size (); ++i)
  {
    values = orig_values;
    v.append (std::move (values));
  }

  const auto length_check_save = v;

  values = orig_values;
  try
  {
    EXPECT_THROW (v.append (std::move (values)));
  }
  catch (const std::length_error&)
  {
    CHECK (values == orig_values);
    CHECK (v == length_check_save);
  }

  return 0;
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  return test_exceptions () || test_normal ();
}
