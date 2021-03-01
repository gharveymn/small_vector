INITIALIZE_DEPENDENCY(doctest)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(TEST_NAME ${PACKAGE_NAME})
SET(SOURCE_FILES main.cpp)

SET(TARGET_VERSIONS 11 14 17 20)

SET(COMPILE_OPTS)
SET(LINK_OPTS   )
SET(LINK_LIBS doctest::doctest)

IF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # Clang options
  LIST(APPEND COMPILE_OPTS
                -stdlib=libc++
                -ftime-trace
                -fno-rtti
                --coverage
                -Weverything
                -Wno-padded
                -Wno-c++98-compat
                -Wno-c++98-compat-pedantic
      )
  LIST(APPEND LINK_OPTS
                -Wl,--allow-multiple-definition
                -fno-rtti
                --coverage
      )
  LIST(APPEND LINK_LIBS
                c++
                c++abi
      )
ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # GCC options
  LIST(APPEND COMPILE_OPTS
                -fno-rtti
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
      )
  LIST(APPEND LINK_OPTS)
  LIST(APPEND LINK_LIBS)
ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # MSVC options
  LIST(APPEND COMPILE_OPTS
                /W4
                /GR-
      )
  LIST(APPEND LINK_OPTS)
  LIST(APPEND LINK_LIBS)
ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # Intel options
  LIST(APPEND COMPILE_OPTS)
  LIST(APPEND LINK_OPTS   )
  LIST(APPEND LINK_LIBS   )
ENDIF ()

IF (UNIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  LIST(APPEND COMPILE_OPTS -fsanitize=address)
  LIST(APPEND LINK_OPTS    -fsanitize=address)
ENDIF()

FOREACH (version ${TARGET_VERSIONS})
  SET(target_name ${TEST_NAME}.doctest.c++${version})
  ADD_EXECUTABLE(${target_name} ${SOURCE_FILES})

  TARGET_COMPILE_OPTIONS(${target_name} PRIVATE ${COMPILE_OPTS})
  TARGET_LINK_OPTIONS   (${target_name} PRIVATE ${LINK_OPTS})
  TARGET_LINK_LIBRARIES (${target_name} PRIVATE ${PACKAGE_NAME} ${LINK_LIBS})

  SET_TARGET_PROPERTIES(${target_name} PROPERTIES
                                         CXX_STANDARD          ${version}
                                         CXX_STANDARD_REQUIRED NO
                                         CXX_EXTENSIONS        NO
                       )
ENDFOREACH ()
