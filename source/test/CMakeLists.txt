set (GCH_SMALL_VECTOR_SOURCE_TEST_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  string (REGEX REPLACE "/[wW]([0-4deovX]|all) ?" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#  string (REGEX REPLACE "/GR ?" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()

if (NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
  set (CMAKE_CXX_STANDARD_REQUIRED OFF)
endif ()

if (NOT DEFINED CMAKE_CXX_EXTENSIONS)
  set (CMAKE_CXX_EXTENSIONS OFF)
endif ()

add_library (small_vector.test.test_common INTERFACE)

target_include_directories (small_vector.test.test_common INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries (small_vector.test.test_common INTERFACE gch::small_vector)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options (
    small_vector.test.test_common
    INTERFACE
#      -fno-rtti
      -ftime-trace
      --coverage
      -Weverything
      -Wno-padded
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-zero-as-null-pointer-constant
      -Wno-used-but-marked-unused
      -Wno-weak-vtables
      -Wno-exit-time-destructors
  )

  target_link_options (
    small_vector.test.test_common
    INTERFACE
#      -fno-rtti
      --coverage
  )

  if (GCH_USE_LIBCXX_WITH_CLANG)
    target_compile_options (small_vector.test.test_common INTERFACE -stdlib=libc++)
    target_link_options (small_vector.test.test_common INTERFACE LINKER:--allow-multiple-definition)
    target_link_libraries (small_vector.test.test_common INTERFACE c++ c++abi)
  endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options (
    small_vector.test.test_common
    INTERFACE
#      -fno-rtti
      -pedantic
      -Wall
      -Wextra
      -Wcast-align
      -Wcast-qual
      -Wctor-dtor-privacy
      -Wdisabled-optimization
      -Wformat=2
      -Winit-self
      -Wlogical-op
      -Wmissing-declarations
      -Wmissing-include-dirs
      -Wnoexcept
      -Wold-style-cast
      -Woverloaded-virtual
      -Wredundant-decls
      -Wshadow
      -Wsign-conversion
      -Wsign-promo
      -Wstrict-null-sentinel
      -Wstrict-overflow=5
      -Wswitch-default
      -Wundef
      -Wno-unused
      -fconcepts-diagnostics-depth=10
  )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options (
    small_vector.test.test_common
    INTERFACE
      /W4
#      /GR-
  )
endif ()

if (UNIX)
  target_compile_options (
    small_vector.test.test_common
    INTERFACE
      $<$<CONFIG:Debug>:-fsanitize=address>
  )

  target_link_options (
    small_vector.test.test_common
    INTERFACE
      $<$<CONFIG:Debug>:-fsanitize=address>
  )
endif ()

if (GCH_SMALL_VECTOR_TEST_ENABLE_REL_OPS_TESTS)
  target_compile_definitions (
    small_vector.test.test_common
    INTERFACE
      GCH_SMALL_VECTOR_TEST_REL_OPS_AMBIGUITY
  )
endif ()

add_custom_target (small_vector.ctest)

macro (add_small_vector_ctest_executables)
  foreach (file ${ARGN})
    get_filename_component (_TARGET_NAME_BASE "${file}" NAME_WE)
    string (PREPEND _TARGET_NAME_BASE "small_vector.")

    foreach (version 11 14 17 20)
      set (_TARGET_NAME ${_TARGET_NAME_BASE}.c++${version})

      add_executable (${_TARGET_NAME} ${file})
      target_link_libraries (${_TARGET_NAME} PRIVATE small_vector.test.test_common)

      target_compile_features (${_TARGET_NAME} PRIVATE cxx_std_${version})
      add_dependencies (small_vector.ctest ${_TARGET_NAME})

      add_test (
        NAME
          ${_TARGET_NAME}
        COMMAND
          ${_TARGET_NAME}
      )

      set (_TARGET_NAME ${_TARGET_NAME_BASE}.no_exceptions.c++${version})

      add_executable (${_TARGET_NAME} ${file})
      target_link_libraries (${_TARGET_NAME} PRIVATE small_vector.test.test_common)

      target_compile_features (${_TARGET_NAME} PRIVATE cxx_std_${version})
      target_compile_options (
        ${_TARGET_NAME}
        PRIVATE
          $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang>:-fno-exceptions>
          $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/EHs->
      )
    endforeach ()
  endforeach ()
endmacro ()

add_small_vector_ctest_executables (
  main.cpp
)

add_subdirectory (bench)
add_subdirectory (unit)
