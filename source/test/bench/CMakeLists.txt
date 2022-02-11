set (Boost_NO_WARN_NEW_VERSIONS ON)
find_package (Boost QUIET COMPONENTS container)
find_package (LLVM  QUIET CONFIG)
find_package (folly QUIET)

foreach (version 11 14 17 20)
  add_small_vector_test_executable (
    small_vector.bench.c++${version}
    bench.cpp
    graphs.cpp
    demangle.cpp)

  target_compile_definitions (
    small_vector.bench.c++${version}
    PRIVATE
      GCH_UNRESTRICTED_DEFAULT_BUFFER_SIZE
  )

  if (Boost_FOUND)
    target_link_libraries (small_vector.bench.c++${version} PRIVATE Boost::container)
    target_compile_definitions (small_vector.bench.c++${version} PRIVATE GCH_WITH_BOOST)
  endif ()

  if (LLVM_FOUND)
    target_link_libraries (small_vector.bench.c++${version} PRIVATE LLVMSupport)
    target_compile_definitions (small_vector.bench.c++${version} PRIVATE GCH_WITH_LLVM)
  endif ()

  if (FOLLY_FOUND)
    target_link_libraries (small_vector.bench.c++${version} PRIVATE Folly::folly)
    target_compile_definitions (small_vector.bench.c++${version} PRIVATE GCH_WITH_FOLLY)
  endif ()

  target_compile_options (
    small_vector.bench.c++${version}
    PRIVATE
      $<$<COMPILE_LANG_AND_ID:CXX,Clang,GNU>:-Wno-missing-field-initializers>
  )

  set_target_properties (
    small_vector.bench.c++${version}
    PROPERTIES
    CXX_STANDARD
      ${version}
    CXX_STANDARD_REQUIRED
      NO
    CXX_EXTENSIONS
      NO
  )
endforeach ()

if (Boost_FOUND)

else ()
  message (WARNING "Could not find Boost.")
endif ()
