set(PARSER_Y   ${CMAKE_CURRENT_SOURCE_DIR}/masm.y)
set(PARSER_H   ${CMAKE_CURRENT_BINARY_DIR}/masm.h)
set(PARSER_C   ${CMAKE_CURRENT_BINARY_DIR}/masm.c)
set(PARSER_OUT ${CMAKE_CURRENT_BINARY_DIR}/masm.out)

add_custom_command(
  OUTPUT ${PARSER_C} ${PARSER_H}
  COMMAND $<TARGET_FILE:lemon> -d${CMAKE_CURRENT_BINARY_DIR} -T${LEMPAR} ${PARSER_Y}
  DEPENDS ${PARSER_Y} lemon
  BYPRODUCTS ${PARSER_OUT}
  VERBATIM
)
add_custom_target(generate_parser
        DEPENDS ${PARSER_C} ${PARSER_H}
)

find_package (Python3 REQUIRED COMPONENTS Interpreter)
set(KEYWORDS_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/keywords.txt)
set(MASM_INPUT   ${PARSER_H})
set(PP_OUT       ${CMAKE_CURRENT_BINARY_DIR}/masm_pp.h)
set(UNITTEST_OUT ${CMAKE_CURRENT_BINARY_DIR}/test_keywords.h)
set(RE_OUT       ${CMAKE_CURRENT_BINARY_DIR}/lexer_keywords.h)
set(KW_OUT       ${CMAKE_CURRENT_BINARY_DIR}/lexer_kw_keywords.h)

add_custom_command(
        OUTPUT  ${PP_OUT} ${UNITTEST_OUT} ${RE_OUT} ${KW_OUT}
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/template_generator.py -m ${MASM_INPUT} -t ${KEYWORDS_INPUT} -p ${PP_OUT}
        DEPENDS ${KEYWORDS_INPUT} ${PARSER_C} tools/template_generator.py
        VERBATIM)
add_custom_target(generate_utility
        DEPENDS  ${PP_OUT} ${UNITTEST_OUT} ${RE_OUT} ${KW_OUT}
        )

set(LEXER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/masm_lexer.re)
set(LEXER_OUT ${CMAKE_CURRENT_BINARY_DIR}/masm_lexer.c)

add_custom_command(
        OUTPUT ${LEXER_OUT}
        COMMAND $<TARGET_FILE:lexical::re2c> --case-insensitive -I${CMAKE_CURRENT_BINARY_DIR} -W -s -o ${LEXER_OUT} ${LEXER_SRC}
        DEPENDS ${LEXER_SRC} ${KW_OUTPUT} ${RE_OUTPUT} re2c
)
add_custom_target(generate_lexer
        DEPENDS ${LEXER_OUT}
        )

set (GENERATED_SOURCES
        ${PARSER_C}
        ${PARSER_H}
        ${LEXER_OUT}
        ${PP_OUT}
        ${UNITTEST_OUT}
        ${RE_OUT}
        ${KW_OUT}
)

set(TEST_SOURCES
        test/lexer_utils.cc
        test/lexer_utils.hh

        test/test_masm_main.cc
        test/test_lexer.cc
        test/test_parser.cc
        )

add_library(masm-parser
        SHARED
        ${GENERATED_SOURCES}
        masm_lexer.h
        externc.h
        ${PROJECT_INCLUDE}/masm/masm-parser.hh
        masm_parser_driver.cc
        masm_parser_iface.h
        masm_parser_iface.cc
        pretty_print.cc
        pretty_print.hh
        ${TEST_SOURCES}
        )

target_link_libraries(masm-parser
        PRIVATE
        thirdparty::doctest
        thirdparty::mio
        )

target_include_directories(masm-parser
        PUBLIC
          ${PROJECT_INCLUDE}
        PRIVATE
          ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}
        )
add_dependencies(masm-parser generate_parser generate_lexer generate_utility)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
generate_export_header(masm-parser)