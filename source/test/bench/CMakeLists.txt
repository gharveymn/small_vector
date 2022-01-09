set (Boost_NO_WARN_NEW_VERSIONS ON)
find_package (Boost COMPONENTS container)

if (Boost_FOUND)
  foreach (version 11 14 17 20)
    add_unit_test (small_vector.bench.c++${version} bench.cpp graphs.cpp demangle.cpp)

    target_compile_definitions (
      small_vector.bench.c++${version}
      PRIVATE
        GCH_UNRESTRICTED_DEFAULT_BUFFER_SIZE
    )

    target_link_libraries (small_vector.bench.c++${version} PRIVATE Boost::container)

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
else ()
  message (WARNING "Could not find Boost.")
endif ()
