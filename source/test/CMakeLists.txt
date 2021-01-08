SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(TEST_NAME ${PACKAGE_NAME})
SET(SOURCE_FILES main.cpp)

SET(TARGET_VERSIONS 11 14 17 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET(COMPILE_OPTS -stdlib=libc++ -ftime-trace -fno-rtti --coverage -Weverything -Wno-c++98-compat)
  SET(LINK_OPTS -Wl,--allow-multiple-definition -fno-rtti --coverage)
  SET(LINK_LIBS c++ c++abi)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET(COMPILE_OPTS -fno-rtti -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused)
  SET(LINK_OPTS )
  SET(LINK_LIBS )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  SET(COMPILE_OPTS /W4 /GR-)
  SET(LINK_OPTS )
  SET(LINK_LIBS )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  SET(COMPILE_OPTS )
  SET(LINK_OPTS )
  SET(LINK_LIBS )
endif()

IF (UNIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  LIST(APPEND COMPILE_OPTS -fsanitize=address)
  LIST(APPEND LINK_OPTS -fsanitize=address)
ENDIF()

FOREACH(version ${TARGET_VERSIONS})
  SET(target_name test.${TEST_NAME}.c++${version})
  ADD_EXECUTABLE(${target_name} ${SOURCE_FILES})

  TARGET_COMPILE_OPTIONS(${target_name} PUBLIC ${COMPILE_OPTS})
  TARGET_LINK_OPTIONS   (${target_name} PUBLIC ${LINK_OPTS})
  TARGET_LINK_LIBRARIES (${target_name} ${PACKAGE_NAME} ${LINK_LIBS})

  SET_TARGET_PROPERTIES(${target_name} PROPERTIES
                        CXX_STANDARD          ${version}
                        CXX_STANDARD_REQUIRED NO
                        CXX_EXTENSIONS        NO)
ENDFOREACH()
