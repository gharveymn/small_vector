find_package (doctest)

if (doctest_FOUND)
  foreach (version 11 14 17 20)
    add_test_executable (small_vector.doctest.c++${version} main.cpp)

    target_link_libraries (small_vector.doctest.c++${version} PRIVATE doctest::doctest)

    set_target_properties (
      small_vector.doctest.c++${version}
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
  message (WARNING "Could not find doctest.")
endif ()
