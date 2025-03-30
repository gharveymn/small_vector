foreach (version ${GCH_SMALL_VECTOR_TEST_STANDARD_VERSIONS})
  set (_TARGET_NAME small_vector.test.integration.boost.pmr_allocator.c++${version})
  add_executable (${_TARGET_NAME} pmr_allocator.cpp)
  target_link_libraries (${_TARGET_NAME} PRIVATE gch::small_vector Boost::boost)
  target_compile_options (${_TARGET_NAME} PRIVATE "$<$<COMPILE_LANG_AND_ID:CXX,GNU>:-fconcepts-diagnostics-depth=100>")
  set_target_properties (
    ${_TARGET_NAME}
    PROPERTIES
    CXX_STANDARD
      ${version}
    CXX_STANDARD_REQUIRED
      NO
    CXX_EXTENSIONS
      NO
  )
endforeach ()
