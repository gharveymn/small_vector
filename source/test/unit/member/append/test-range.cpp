/** test-range.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"
#include "test_allocators.hpp"

#include <numeric>

template <typename Iter>
static GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_exceptions (Iter first, Iter last)
{
  using namespace gch;

  using test_type = test_types::triggering_copy_ctor;

  small_vector_with_allocator<test_type, test_types::sized_allocator<test_type, std::uint8_t>> v;

  auto range_len = static_cast<std::size_t> (std::distance (first, last));

  // Append an empty range.
  v.append (first, first);
  v.append (last, last);

  CHECK (v.empty ());

  v.append (first, last);

  CHECK (v.size () == range_len);
  CHECK (std::equal (v.begin (), v.end (), first));

  CHECK (std::numeric_limits<std::uint8_t>::max () / sizeof (test_type) == v.max_size ());
  v.reserve (v.max_size ());

  const auto save = v;

  // Throw at the very beginning.
  test_types::global_exception_trigger.reset (1);
  try
  {
    EXPECT_THROW (v.append (first, last));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (v == save);
  }

  // Throw in the middle.
  test_types::global_exception_trigger.reset (range_len / 2);
  try
  {
    EXPECT_THROW (v.append (first, last));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (v == save);
  }

  // Throw at the end.
  test_types::global_exception_trigger.reset (range_len);
  try
  {
    EXPECT_THROW (v.append (first, last));
  }
  catch (const test_types::test_exception&)
  {
    CHECK (v == save);
  }

  v.clear ();
  v.shrink_to_fit ();

  for (std::size_t i = 0; i < v.max_size () / range_len; ++i)
    v.append (first, last);

  const auto length_check_save = v;

  try
  {
    EXPECT_THROW (v.append (first, last));
  }
  catch (const std::length_error&)
  {
    CHECK (v == length_check_save);
  }

  return 0;
}

template <typename Iter>
static GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test_conversion (Iter first, Iter last)
{
  using namespace gch;

  using test_type = test_types::triggering_copy_ctor;

  small_vector_with_allocator<test_type, test_types::sized_allocator<test_type, std::uint8_t>> v;

  auto range_len = static_cast<std::size_t> (std::distance (first, last));

  // Append an empty range.
  v.append (first, first);
  v.append (last, last);

  CHECK (v.empty ());

  v.append (first, last);

  CHECK (v.size () == range_len);
  CHECK (std::equal (v.begin (), v.end (), first));

  CHECK (std::numeric_limits<std::uint8_t>::max () / sizeof (test_type) == v.max_size ());
  v.reserve (v.max_size ());

  v.clear ();
  v.shrink_to_fit ();

  for (std::size_t i = 0; i < v.max_size () / range_len; ++i)
    v.append (first, last);

  const auto length_check_save = v;

  try
  {
    EXPECT_THROW (v.append (first, last));
  }
  catch (const std::length_error&)
  {
    CHECK (v == length_check_save);
  }

  return 0;
}

template <typename Iter>
class input_iterator_wrapper;

template <typename T>
class input_iterator_wrapper<T *>
{
public:
  using difference_type   = typename std::iterator_traits<T *>::difference_type;
  using value_type        = typename std::iterator_traits<T *>::value_type;
  using pointer           = typename std::iterator_traits<T *>::pointer;
  using reference         = typename std::iterator_traits<T *>::reference;
  using iterator_category = std::input_iterator_tag;

  input_iterator_wrapper            (void)                              = default;
  input_iterator_wrapper            (const input_iterator_wrapper&)     = default;
  input_iterator_wrapper            (input_iterator_wrapper&&) noexcept = default;
  input_iterator_wrapper& operator= (const input_iterator_wrapper&)     = default;
  input_iterator_wrapper& operator= (input_iterator_wrapper&&) noexcept = default;
  ~input_iterator_wrapper           (void)                              = default;

  constexpr
  input_iterator_wrapper (T *ptr) noexcept
    : m_iter (ptr)
  { }

  constexpr
  reference
  operator* (void) const noexcept
  {
    return *m_iter;
  }

  constexpr
  pointer
  operator-> (void) const noexcept
  {
    return m_iter;
  }

  GCH_CPP14_CONSTEXPR
  input_iterator_wrapper&
  operator++ (void) noexcept
  {
    ++m_iter;
    return *this;
  }

  GCH_CPP14_CONSTEXPR
  input_iterator_wrapper
  operator++ (int) noexcept
  {
    return input_iterator_wrapper (m_iter++);
  }

private:
  T *m_iter = nullptr;
};

template <typename T>
constexpr
bool
operator!= (input_iterator_wrapper<T *> lhs, input_iterator_wrapper<T *> rhs) noexcept
{
  return lhs.operator-> () != rhs.operator-> ();
}

template <typename T>
constexpr
input_iterator_wrapper<T *>
wrap_iter (T *ptr)
{
  return input_iterator_wrapper<T *> { ptr };
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  gch::test_types::triggering_copy_ctor values[5] { 1, 2, 3, 4, 5 };
  int ivalues[5] { 1, 2, 3, 4, 5 };

  CHECK (0 == test_exceptions (std::begin (values), std::end (values)));
  CHECK (0 == test_conversion (std::begin (ivalues), std::end (ivalues)));
  CHECK (0 == test_exceptions (wrap_iter (std::begin (values)), wrap_iter (std::end (values))));
  CHECK (0 == test_conversion (wrap_iter (std::begin (ivalues)), wrap_iter (std::end (ivalues))));

  return 0;
}
