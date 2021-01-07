IF(NOT CREATE_PACKAGE_VERSION VERSION_GREATER_EQUAL 0.0.4)
  IF(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/create-package/cmake/create-package.cmake")
    MESSAGE(STATUS "Initializing create-package-0.0.4 in git submodule.")
    EXECUTE_PROCESS(COMMAND git submodule --quiet update --init -- "${PROJECT_SOURCE_DIR}/external/create-package"
                    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
  ENDIF()
  INCLUDE("${PROJECT_SOURCE_DIR}/external/create-package/cmake/create-package.cmake")
ENDIF()

CREATE_PACKAGE(AUTHOR       gharveymn
               NAMESPACE    gch
               NAME         small_vector
               HEADERS      small_vector.hpp)

# Only include tests if we are at the top level
IF(SMALL_VECTOR_BUILD_TESTS)
  MESSAGE(STATUS "Testing is enabled.")
  ADD_SUBDIRECTORY(test)
  ADD_SUBDIRECTORY(bench)
ELSE()
  MESSAGE(STATUS "Testing is disabled.")
ENDIF()
