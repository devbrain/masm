//
// Created by igor on 26/07/2022.
//

#include <doctest/doctest.h>
#include "masm_parser_iface.h"

TEST_SUITE("Parser") {
  TEST_CASE(".MODEL") {

    auto ast = masm::parse (".MODEL SMALL\n");
    REQUIRE(ast);
    REQUIRE(ast->the_memory_model.model == MEMORY_MODEL_SMALL);

    ast = masm::parse (".MODEL HUGE\n");
    REQUIRE(ast);
    REQUIRE(ast->the_memory_model.model == MEMORY_MODEL_HUGE);

    ast = masm::parse (".MODEL FLAT, PASCAL, STDCALL, NEARSTACK\n");
    REQUIRE(ast);
    REQUIRE(ast->the_memory_model.model == MEMORY_MODEL_FLAT);
    REQUIRE(ast->the_memory_model.language_type == LANGUAGE_TYPE_PASCAL);
    REQUIRE(ast->the_memory_model.call_type == CALL_TYPE_STDCALL);
    REQUIRE(ast->the_memory_model.stack_option == STACK_OPTION_NEARSTACK);
  }

  TEST_CASE("NAME") {
    auto ast = masm::parse ("NAME pizda\n", true);
  }
}