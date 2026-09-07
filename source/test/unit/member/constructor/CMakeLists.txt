add_small_vector_unit_tests (
  test-copy.cpp
  test-default.cpp
  test-elem.cpp
  test-generator.cpp
  test-ilist.cpp
  test-move.cpp
  test-range-legacy.cpp
)

add_small_vector_unit_tests (
  MINIMUM_VERSION 23
  test-range.cpp
)
