set(PYBIND11_TEST OFF CACHE INTERNAL "")
fetchcontent_declare(
        PYBIND
        GIT_REPOSITORY "https://github.com/pybind/pybind11.git"
        UPDATE_COMMAND ""
)
fetchcontent_makeavailable(PYBIND)
add_library(thirdparty::pybind ALIAS pybind11_headers)
