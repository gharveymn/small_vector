add_library (small_vector INTERFACE)

target_sources (
  small_vector
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/gch/small_vector.hpp>
)

target_include_directories (
  small_vector
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_include_directories (
  small_vector
  SYSTEM INTERFACE
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

set_target_properties (
  small_vector
  PROPERTIES
  PUBLIC_HEADER
    include/gch/small_vector.hpp
)

add_library (gch::small_vector ALIAS small_vector)

install (
  TARGETS
    small_vector
  EXPORT
    small_vector-targets
  PUBLIC_HEADER
    DESTINATION include/gch
)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/small_vector-config.cmake [=[
get_filename_component (PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
include ("${CMAKE_CURRENT_LIST_DIR}/small_vector-targets.cmake")
]=])

include (CMakePackageConfigHelpers)
write_basic_package_version_file (
  small_vector-version.cmake
  COMPATIBILITY
    SameMinorVersion
)

install (
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/small_vector-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/small_vector-version.cmake
  DESTINATION
    lib/cmake/small_vector
)

install (
  EXPORT
    small_vector-targets
  DESTINATION
    lib/cmake/small_vector
  NAMESPACE
    gch::
)

# Only include tests if we are at the top level
if (GCH_SMALL_VECTOR_BUILD_TESTS)
  add_subdirectory (test)
endif ()
