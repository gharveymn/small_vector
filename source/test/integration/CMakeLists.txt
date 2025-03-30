find_package (Boost CONFIG QUIET COMPONENTS interprocess)
if (Boost_FOUND)
  add_subdirectory (boost)
endif ()
