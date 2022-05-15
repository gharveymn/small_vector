if (GCH_SMALL_VECTOR_BENCH_ENABLE_BOOST OR NOT DEFINED GCH_SMALL_VECTOR_BENCH_ENABLE_BOOST)
  set (Boost_NO_WARN_NEW_VERSIONS ON)

  if (GCH_SMALL_VECTOR_BENCH_ENABLE_BOOST)
    find_package (Boost COMPONENTS container)

    if (NOT Boost_FOUND)
      message (WARNING "Could not find Boost. Setting GCH_SMALL_VECTOR_BENCH_ENABLE_BOOST to OFF.")
      set_property (CACHE GCH_SMALL_VECTOR_BENCH_ENABLE_BOOST PROPERTY VALUE OFF)
    endif ()
  else ()
    find_package (Boost QUIET COMPONENTS container)

    option (
      GCH_SMALL_VECTOR_BENCH_ENABLE_BOOST
      "Enable benchmark comparisons with Boost."
      "${Boost_FOUND}"
    )
  endif ()
endif ()

if (GCH_SMALL_VECTOR_BENCH_ENABLE_LLVM OR NOT DEFINED GCH_SMALL_VECTOR_BENCH_ENABLE_LLVM)
  enable_language (C)
  if (GCH_SMALL_VECTOR_BENCH_ENABLE_LLVM)
    find_package (LLVM CONFIG)

    if (NOT LLVM_FOUND)
      message (WARNING "Could not find LLVM. Setting GCH_SMALL_VECTOR_BENCH_ENABLE_LLVM to OFF.")
      set_property (CACHE GCH_SMALL_VECTOR_BENCH_ENABLE_LLVM PROPERTY VALUE OFF)
    endif ()
  else ()
    find_package (LLVM QUIET CONFIG)

    option (
      GCH_SMALL_VECTOR_BENCH_ENABLE_LLVM
      "Enable benchmark comparisons with LLVM."
      "${LLVM_FOUND}"
    )
  endif ()
endif ()

if (GCH_SMALL_VECTOR_BENCH_ENABLE_FOLLY OR NOT DEFINED GCH_SMALL_VECTOR_BENCH_ENABLE_FOLLY)
  if (GCH_SMALL_VECTOR_BENCH_ENABLE_FOLLY)
    find_package (folly)

    if (NOT folly_FOUND)
      message (WARNING "Could not find folly. Setting GCH_SMALL_VECTOR_BENCH_ENABLE_FOLLY to OFF.")
      set_property (CACHE GCH_SMALL_VECTOR_BENCH_ENABLE_FOLLY PROPERTY VALUE OFF)
    endif ()
  else ()
    find_package (folly QUIET)

    option (
      GCH_SMALL_VECTOR_BENCH_ENABLE_FOLLY
      "Enable benchmark comparisons with folly."
      "${folly_FOUND}"
    )
  endif ()
endif ()

foreach (version ${GCH_SMALL_VECTOR_TEST_STANDARD_VERSIONS})
  add_executable (
    small_vector.bench.c++${version}
    bench.cpp
    graphs.cpp
    demangle.cpp
  )

  target_link_libraries (small_vector.bench.c++${version} PRIVATE small_vector.test.test_common)

  target_compile_definitions (
    small_vector.bench.c++${version}
    PRIVATE
      GCH_UNRESTRICTED_DEFAULT_BUFFER_SIZE
  )

  if (GCH_SMALL_VECTOR_BENCH_ENABLE_BOOST)
    target_link_libraries (small_vector.bench.c++${version} PRIVATE Boost::container)
    target_compile_definitions (small_vector.bench.c++${version} PRIVATE GCH_WITH_BOOST)
  endif ()

  if (GCH_SMALL_VECTOR_BENCH_ENABLE_LLVM AND version GREATER 11)
    target_link_libraries (small_vector.bench.c++${version} PRIVATE LLVMSupport)
    target_compile_definitions (small_vector.bench.c++${version} PRIVATE GCH_WITH_LLVM)
    if (LLVM_INCLUDE_DIRS)
      target_include_directories (
        small_vector.bench.c++${version}
        SYSTEM PRIVATE
          ${LLVM_INCLUDE_DIRS}
      )
    endif ()
  endif ()

  if (GCH_SMALL_VECTOR_BENCH_ENABLE_FOLLY)
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
