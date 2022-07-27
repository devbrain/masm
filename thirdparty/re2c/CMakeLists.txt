SET(RE2C_BUILD_RE2GO OFF CACHE INTERNAL "")

fetchcontent_declare(
        RE2C
        URL https://github.com/skvadrik/re2c/archive/refs/tags/2.2.zip
)

fetchcontent_makeavailable(RE2C)
add_executable(lexical::re2c ALIAS re2c)
