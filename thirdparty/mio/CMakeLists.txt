set(mio.tests OFF CACHE INTERNAL "")

fetchcontent_declare(
        MIO
        GIT_REPOSITORY https://github.com/devbrain/mio.git
        UPDATE_COMMAND ""
)

fetchcontent_makeavailable(MIO)
add_library(thirdparty::mio ALIAS mio)
set_target_properties(mio PROPERTIES FOLDER "ThirdParty")
