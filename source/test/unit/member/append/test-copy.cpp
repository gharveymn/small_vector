/** test-copy.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  using namespace gch;

  using test_type = test_types::triggering_copy_ctor;
  using alloc_type = test_types::sized_allocator<test_type, std::uint8_t>;

  small_vector<test_type, 5, alloc_type> values;
  small_vector<test_type, 6, alloc_type> v;

  // Append an empty range.
  v.append (values);

  CHECK (v.empty ());

  values.assign ({ 1, 2, 3, 4, 5 });
  v.append (values);

  CHECK (v.size () == values.size ());
  CHECK (std::equal (v.begin (), v.end (), values.begin ()));

  CHECK (std::numeric_limits<std::uint8_t>::max () / sizeof (test_type) == v.max_size ());
  v.reserve (v.max_size ());

  const auto save_values = values;
  const auto save = v;

  // Throw at the very beginning.
  test_types::global_exception_trigger.reset (1);
  try
  {
    EXPECT_THROW (v.append (values));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (values == save_values);
    CHECK (v == save);
  }

  // Throw in the middle.
  test_types::global_exception_trigger.reset (values.size () / 2);
  try
  {
    EXPECT_THROW (v.append (values));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (values == save_values);
    CHECK (v == save);
  }

  // Throw at the end.
  test_types::global_exception_trigger.reset (values.size ());
  try
  {
    EXPECT_THROW (v.append (values));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (values == save_values);
    CHECK (v == save);
  }

  v.clear ();
  v.shrink_to_fit ();

  for (std::size_t i = 0; i < v.max_size () / values.size (); ++i)
    v.append (values.begin (), values.end ());

  const auto length_check_save = v;

  try
  {
    EXPECT_THROW (v.append (values));
  }
  catch (const std::length_error&)
  {
    CHECK (values == save_values);
    CHECK (v == length_check_save);
  }

  return 0;
}
