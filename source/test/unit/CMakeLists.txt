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

foreach (version 11 14 17 20)
  set (_TARGET_NAME small_vector.test.unit.main.c++${version})
  add_library (${_TARGET_NAME} OBJECT)
  target_sources (${_TARGET_NAME} PRIVATE main.cpp)
  target_link_libraries (${_TARGET_NAME} PUBLIC small_vector.test.unit.unit_test_common)
  target_compile_features (${_TARGET_NAME} PUBLIC cxx_std_${version})

  if (GCH_SMALL_VECTOR_TEST_ENABLE_PCH)
    target_precompile_headers (${_TARGET_NAME} PUBLIC [["gch/small_vector.hpp"]])
  endif ()
endforeach ()

# Check if constexpr small_vector is available.
if (NOT DEFINED GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR)
  file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/test.cpp [=[
  #include "gch/small_vector.hpp"
  constexpr int test (void) noexcept { gch::small_vector<int> v; return 0; }
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
      -DGCH_SMALL_VECTOR_TEST_FILE=${CMAKE_CURRENT_BINARY_DIR}/test.cpp
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

function (add_small_vector_unit_tests)

  cmake_parse_arguments (
    asvut
    "EXPECT_FAIL;COMPILATION_TEST;ONLY_CONSTEXPR;NO_CONSTEXPR"
    "MINIMUM_VERSION;SUFFIX"
    "FILES;COMPILE_DEFINITIONS;LINK_LIBRARIES"
    ${ARGN}
  )

  list (APPEND asvut_FILES ${asvut_UNPARSED_ARGUMENTS})

  if (asvut_ONLY_CONSTEXPR)
    set (asvut_COMPILATION_TEST    ON)
    set (asvut_SUFFIX              .constexpr)
    set (asvut_MINIMUM_VERSION     20)
    set (asvut_COMPILE_DEFINITIONS GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR)
  endif ()

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
      string (APPEND _TARGET_NAME_BASE ".${_FILENAME_WE}")
    endif ()

    foreach (version ${_asvut_VERSIONS})
      set (_TARGET_NAME "${_TARGET_NAME_BASE}.c++${version}${asvut_SUFFIX}")

      if (asvut_COMPILATION_TEST)
        if (asvut_ONLY_CONSTEXPR)
          add_executable (${_TARGET_NAME})

          target_sources (${_TARGET_NAME} PRIVATE ${GCH_SMALL_VECTOR_SOURCE_TEST_UNIT_DIR}/main.cpp)
          get_source_file_property (_FILE_LOCATION ${file} LOCATION)
          target_compile_definitions (
            ${_TARGET_NAME}
            PRIVATE
              GCH_SMALL_VECTOR_TEST_FILE=${_FILE_LOCATION}
          )

          target_compile_options (
            ${_TARGET_NAME}
            PRIVATE
              $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-fconstexpr-ops-limit=2147483647>
              $<$<COMPILE_LANG_AND_ID:CXX,Clang>:-fconstexpr-steps=2147483647>
              $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/constexpr:steps2147483647>
          )
        else ()
          add_library (${_TARGET_NAME} OBJECT)
          target_sources (${_TARGET_NAME} PRIVATE ${file})
        endif ()

        target_link_libraries (${_TARGET_NAME} PRIVATE small_vector.test.unit.unit_test_common)

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
      else ()
        add_executable (${_TARGET_NAME} ${file})
        target_link_libraries (${_TARGET_NAME} PRIVATE small_vector.test.unit.main.c++${version})

        add_dependencies (small_vector.ctest ${_TARGET_NAME})

        add_test (
          NAME
            ${_TARGET_NAME}
          COMMAND
            ${_TARGET_NAME}
        )

        if (DEFINED asvut_COMPILE_DEFINITIONS OR NOT GCH_SMALL_VECTOR_TEST_ENABLE_PCH)
          set_target_properties (${_TARGET_NAME} PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
        else ()
          set_target_properties (
            ${_TARGET_NAME}
            PROPERTIES
            PRECOMPILE_HEADERS_REUSE_FROM
              small_vector.test.unit.main.c++${version}
          )
        endif ()
      endif ()

      target_compile_features (${_TARGET_NAME} PRIVATE cxx_std_${version})
      target_compile_definitions (${_TARGET_NAME} PRIVATE ${asvut_COMPILE_DEFINITIONS})
      target_link_libraries (${_TARGET_NAME} PRIVATE ${asvut_LINK_LIBRARIES})

      if (asvut_EXPECT_FAIL)
        set_tests_properties (
          ${_TARGET_NAME}
          PROPERTIES
          WILL_FAIL
            ON
        )
      endif ()
    endforeach ()

    if (GCH_SMALL_VECTOR_TEST_HAS_CONSTEXPR AND NOT asvut_ONLY_CONSTEXPR AND NOT asvut_NO_CONSTEXPR)
      add_small_vector_unit_tests (${file} ONLY_CONSTEXPR)
    endif ()
  endforeach ()
endfunction ()

add_subdirectory (instantiation)
add_subdirectory (member)
add_subdirectory (non-member)
