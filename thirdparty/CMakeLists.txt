include(FetchContent)
include(ExternalProject)
find_package(Git REQUIRED)

set(FETCHCONTENT_QUIET off)

add_subdirectory(lemon)
add_subdirectory(re2c)
add_subdirectory(doctest)
add_subdirectory(pybind)
add_subdirectory(mio)