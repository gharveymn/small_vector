add_small_vector_unit_tests (
  test-copy.cpp
  test-elem.cpp
  test-ilist.cpp
  test-move.cpp
  test-range.cpp
)

add_small_vector_unit_tests (
  test-interop.cpp
  NO_CONSTEXPR
  COMPILE_DEFINITIONS
    GCH_STDLIB_INTEROP
)
