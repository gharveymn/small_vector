set (GCH_SMALL_VECTOR_SOURCE_TEST_UNIT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

add_library (small_vector.test.unit.unit_test_common INTERFACE)

target_include_directories (
  small_vector.test.unit.unit_test_common
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries (
  small_vector.test.unit.unit_test_common
  INTERFACE
    small_vector.test.test_common
)

add_library (small_vector.test.unit.main OBJECT)
target_sources (small_vector.test.unit.main PRIVATE main.cpp)
target_link_libraries (small_vector.test.unit.main PUBLIC small_vector.test.unit.unit_test_common)

# Check if constexpr small_vector is available.
if (NOT DEFINED GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR)
  file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/test.cpp [=[
  #include "gch/small_vector.hpp"
  constexpr int test (void) { gch::small_vector<int> v; return v.size (); }
  ]=])

  set (
    _TEST_INCLUDE_DIRS
    ${GCH_SMALL_VECTOR_SOURCE_INCLUDE_DIR}
    ${GCH_SMALL_VECTOR_SOURCE_TEST_DIR}
    ${GCH_SMALL_VECTOR_SOURCE_TEST_UNIT_DIR}
  )

  message (CHECK_START "Checking if the compiler supports constexpr gch::small_vector")

  try_compile (
    GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
    ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
      ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    CMAKE_FLAGS
      "-DINCLUDE_DIRECTORIES=${_TEST_INCLUDE_DIRS}"
    COMPILE_DEFINITIONS
      -DGCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
      -DGCH_SMALL_VECTOR_TEST_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/test.cpp\"
    OUTPUT_VARIABLE
      GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR_OUTPUT
    CXX_STANDARD
      20
    CXX_STANDARD_REQUIRED
      OFF
    CXX_EXTENSIONS
      OFF
  )

  if (GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR)
    message (CHECK_PASS "Success")

    set (GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR 1 CACHE INTERNAL "")
    file (
      APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
      "Test of GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR succeeded with the following output:\n"
      "${GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR_OUTPUT}\n"
    )
  else ()
    message (CHECK_PASS "Failed")

    set (GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR "" CACHE INTERNAL "")
    file (
      APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
      "Test of GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR failed with the following output:\n"
      "${GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR_OUTPUT}\n"
    )
  endif ()
endif ()

macro (add_small_vector_unit_tests)

  cmake_parse_arguments (asvut "EXPECT_FAIL" "MINIMUM_VERSION" "FILES" ${ARGN})

  list (APPEND asvut_FILES ${asvut_UNPARSED_ARGUMENTS})

  set (_asvut_VERSIONS 11 14 17 20)

  if (DEFINED asvut_MINIMUM_VERSION)
    list (GET _asvut_VERSIONS -1 _asvut_LAST)
    if (_asvut_LAST LESS asvut_MINIMUM_VERSION)
      message (FATAL_ERROR "MINIMUM_VERSION specified to add_small_vector_unit_tests is too high.")
    endif ()

    list (GET _asvut_VERSIONS 0 _asvut_FIRST)
    while (_asvut_FIRST LESS asvut_MINIMUM_VERSION)
      list (POP_FRONT _asvut_VERSIONS)
      list (GET _asvut_VERSIONS 0 _asvut_FIRST)
    endwhile ()
  endif ()

  file (
    RELATIVE_PATH _DIR_RELPATH
    ${GCH_SMALL_VECTOR_SOURCE_TEST_UNIT_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
  )

  string (REPLACE "/" "." _DIR_TARGET_BASE "${_DIR_RELPATH}")

  foreach (file ${asvut_FILES})
    set (_TARGET_NAME_BASE "small_vector.test.unit.${_DIR_TARGET_BASE}")
    if (NOT file STREQUAL "test.cpp")
      get_filename_component (_FILENAME_WE "${file}" NAME_WE)
      string (APPEND _TARGET_NAME_BASE "${_FILENAME_WE}")
    endif ()

    foreach (version ${_asvut_VERSIONS})
      set (_TARGET_NAME ${_TARGET_NAME_BASE}.c++${version})

      add_executable (${_TARGET_NAME} ${file})
      target_link_libraries (${_TARGET_NAME} PRIVATE small_vector.test.unit.main)

      add_dependencies (small_vector.ctest ${_TARGET_NAME})
      add_test (
        NAME
          ${_TARGET_NAME}
        COMMAND
          ${_TARGET_NAME}
      )

      if (asvut_EXPECT_FAIL)
        set_tests_properties (
          ${_TARGET_NAME}
          PROPERTIES
          WILL_FAIL
            ON
        )
      endif ()
    endforeach ()

    if (GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR)
      set (_TARGET_NAME ${_TARGET_NAME_BASE}.constexpr)

      add_executable (${_TARGET_NAME} ${GCH_SMALL_VECTOR_SOURCE_TEST_UNIT_DIR}/main.cpp)
      target_link_libraries (${_TARGET_NAME} PRIVATE small_vector.test.unit.unit_test_common)

      target_compile_features (${_TARGET_NAME} PUBLIC cxx_std_20)

      target_compile_definitions (
        ${_TARGET_NAME}
        PRIVATE
          GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR
          GCH_SMALL_VECTOR_TEST_FILE=\"${CMAKE_CURRENT_LIST_DIR}/${file}\"
      )

      set_target_properties (${_TARGET_NAME} PROPERTIES EXCLUDE_FROM_ALL ON)

      add_test (
        NAME
          ${_TARGET_NAME}
        COMMAND
          ${CMAKE_COMMAND}
          --build ${CMAKE_BINARY_DIR}
          --config $<CONFIG>
          --target ${_TARGET_NAME}
          --verbose
      )

      set_tests_properties (
        ${_TARGET_NAME}
        PROPERTIES
        RESOURCE_LOCK
          BUILD_SYSTEM
      )

      if (asvut_EXPECT_FAIL)
        set_tests_properties (
          ${_TARGET_NAME}
          PROPERTIES
          WILL_FAIL
            ON
        )
      endif ()
    endif ()
  endforeach ()
endmacro ()

add_subdirectory (member)
add_subdirectory (non-member)