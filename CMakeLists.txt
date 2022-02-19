cmake_minimum_required (VERSION 3.15)

project (
  small_vector
  VERSION
    0.3.1
  LANGUAGES
    CXX
)

string (COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" _ENABLE_TESTS_DEFAULT)

option (
  GCH_SMALL_VECTOR_ENABLE_TESTS
  "Set to ON to build tests for gch::small_vector."
  ${_ENABLE_TESTS_DEFAULT}
)

if (GCH_SMALL_VECTOR_ENABLE_TESTS)
  include (CMakeDependentOption)

  cmake_dependent_option (
    GCH_USE_LIBCXX_WITH_CLANG
    "Set to ON to use libc++ when compiling with Clang."
    OFF
    "CMAKE_CXX_COMPILER_ID MATCHES Clang"
    OFF
  )

  enable_testing ()
endif ()

set (
  GCH_SMALL_VECTOR_INSTALL_CMAKE_DIR
  "lib/cmake/small_vector"
  CACHE STRING
  "Specify where to install the CMake config files."
)

set (
  GCH_SMALL_VECTOR_INSTALL_INCLUDE_DIR
  "include"
  CACHE STRING
  "Specify where to install gch/small_vector.hpp."
)

set (
  GCH_SMALL_VECTOR_INSTALL_LICENSE_DIR
  "share/licenses/gch/small_vector"
  CACHE STRING
  "Specify where to install the license."
)

set (
  GCH_SMALL_VECTOR_INSTALL_SUPPORT_DIR
  "share/gch"
  CACHE STRING
  "Specify where to install support files like GDB prettyprinters."
)

foreach (dir CMAKE INCLUDE LICENSE SUPPORT)
  if (IS_ABSOLUTE "${GCH_SMALL_VECTOR_INSTALL_${dir}_DIR}")
    message (FATAL_ERROR "GCH_SMALL_VECTOR_INSTALL_${dir}_DIR cannot be absolute.")
  endif ()
endforeach ()

add_subdirectory (source)

export (
  EXPORT
    small_vector-targets
  NAMESPACE
    gch::
)

install (
  FILES
    docs/LICENSE
  DESTINATION
    ${GCH_SMALL_VECTOR_INSTALL_LICENSE_DIR}
)
