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

find_package (Python3 REQUIRED COMPONENTS Interpreter)
set(KEYWORDS_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/keywords.txt)
set(MASM_INPUT ${PARSER_H})
set(PP_OUT ${CMAKE_CURRENT_BINARY_DIR}/masm_pp.h)
set(UNITTEST_OUT ${CMAKE_CURRENT_BINARY_DIR}/test_keywords.h)
set(RE_OUT ${CMAKE_CURRENT_BINARY_DIR}/lexer_keywords.h)
set(KW_OUT ${CMAKE_CURRENT_BINARY_DIR}/lexer_kw_keywords.h)

add_custom_command(
        OUTPUT ${PP_OUT} ${UNITTEST_OUT} ${RE_OUT} ${KW_OUT}
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/template_generator.py -m ${MASM_INPUT} -t ${KEYWORDS_INPUT} -p masm_pp.h
        DEPENDS ${KEYWORDS_INPUT} ${MASM_INPUT} ${CMAKE_CURRENT_SOURCE_DIR}/template_generator.py
        VERBATIM)

MACRO(RE2C VAR SRC)
  GET_FILENAME_COMPONENT(DST "${SRC}" PATH)
  GET_FILENAME_COMPONENT(NAME "${SRC}" NAME_WE)
  SET (ONAME ${CMAKE_CURRENT_BINARY_DIR}/${DST}/${NAME}.c)
  ADD_CUSTOM_COMMAND(
    OUTPUT "${ONAME}"
    BYPRODUCTS ${ONAME}
    COMMAND cmake -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${DST}"
    COMMAND $<TARGET_FILE:lexical::re2c> --case-insensitive -I${CMAKE_CURRENT_BINARY_DIR} -W -s -o ${ONAME} "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}"
    DEPENDS "${SRC}"
    DEPENDS ${ARGN}
    )
  SET(${VAR} ${${VAR}} ${ONAME} ${RE_OUT})
ENDMACRO()

RE2C(MASM_SCANNER masm_lexer.re2c ${PP_OUT} ${RE_OUT} ${KW_OUT})

set(TEST_SOURCES
        test/lexer_utils.cc
        test/lexer_utils.hh

        test/test_masm_main.cc
        test/test_lexer.cc
        test/test_parser.cc
        )

add_library(masm-parser
        SHARED
        ${PARSER_C}
        ${PARSER_H}
        ${MASM_SCANNER}
        ${PP_OUT}
        ${KW_OUT}
        ${UNITTEST_OUT}
        masm_lexer.h
        externc.h
        istream.c
        istream.h
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
        )

target_include_directories(masm-parser
        PUBLIC
          ${PROJECT_INCLUDE}
        PRIVATE
          ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}
        )

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
generate_export_header(masm-parser)