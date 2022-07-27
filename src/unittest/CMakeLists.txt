add_executable(unittest
        main.cc
        )
target_link_libraries(unittest
        PRIVATE
        masm-parser
        thirdparty::doctest
        )