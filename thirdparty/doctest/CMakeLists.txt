set(DOCTEST_WITH_TESTS OFF CACHE INTERNAL "")
set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE INTERNAL "")
fetchcontent_declare(
        DOCTEST
        GIT_REPOSITORY "https://github.com/onqtam/doctest.git"
        UPDATE_COMMAND ""
)
fetchcontent_makeavailable(DOCTEST)
add_library(thirdparty::doctest ALIAS doctest)
