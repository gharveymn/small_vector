CMAKE_MINIMUM_REQUIRED (VERSION 3.13)
PROJECT (small_vector
         VERSION 0.0.2
         LANGUAGES CXX)

IF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  OPTION (GCH_SMALL_VECTOR_BUILD_TESTS "Set to ON to build tests for gch::small_vector" ON)
ELSE ()
  OPTION (GCH_SMALL_VECTOR_BUILD_TESTS "Set to ON to build tests for gch::small_vector" OFF)
ENDIF ()

OPTION (GCH_USE_LIBCXX_WITH_CLANG "Set to ON to use libc++ when compiling with Clang" OFF)

ADD_SUBDIRECTORY (source)
