/** test.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unit_test_common.hpp"

template <typename T>
struct allocator_with_id
  : std::allocator<T>
{
  allocator_with_id            (void)                         = default;
  allocator_with_id            (const allocator_with_id&)     = default;
  allocator_with_id            (allocator_with_id&&) noexcept = default;
  allocator_with_id& operator= (const allocator_with_id&)     = default;
  allocator_with_id& operator= (allocator_with_id&&) noexcept = default;
  ~allocator_with_id           (void)                         = default;

  constexpr explicit
  allocator_with_id (int id) noexcept
    : m_id (id)
  { }

  template <typename U>
  constexpr
  allocator_with_id (const allocator_with_id<U>& other) noexcept
    : std::allocator<T> (other),
      m_id (other.m_id)
  { }

  int m_id = 0;
};

template <typename T, typename U>
constexpr
bool
operator== (const allocator_with_id<T>& lhs, const allocator_with_id<U>& rhs) noexcept
{
  return lhs.m_id == rhs.m_id;
}

template <typename T, typename U>
constexpr
bool
operator!= (const allocator_with_id<T>& lhs, const allocator_with_id<U>& rhs) noexcept
{
  return ! (lhs == rhs);
}

GCH_SMALL_VECTOR_TEST_CONSTEXPR
int
test (void)
{
  gch::small_vector_with_allocator<int, allocator_with_id<int>> v;

  CHECK (v.empty ());
  CHECK (allocator_with_id<int> { } == v.get_allocator ());
  CHECK (allocator_with_id<long> { } == v.get_allocator ());

  allocator_with_id<int> alloc (1);
  gch::small_vector_with_allocator<int, allocator_with_id<int>> w (alloc);

  CHECK (w.empty ());
  CHECK (alloc == w.get_allocator ());
  CHECK (allocator_with_id<int> { } != w.get_allocator ());
  CHECK (v.get_allocator () != w.get_allocator ());

  return 0;
}
