//
// Created by igor on 26/07/2022.
//

#include <doctest/doctest.h>
#include "masm_parser_iface.h"

TEST_SUITE("Parser") {
  TEST_CASE(".MODEL") {
    auto ast = masm::parse (".MODEL SMALL");
    REQUIRE(ast);
    REQUIRE(ast->the_memory_model.model == MEMORY_MODEL_SMALL);

    ast = masm::parse (".MODEL HUGE\n");
    REQUIRE(ast);
    REQUIRE(ast->the_memory_model.model == MEMORY_MODEL_HUGE);

    ast = masm::parse (".MODEL FLAT, PASCAL, STDCALL, NEARSTACK");
    REQUIRE(ast);
    REQUIRE(ast->the_memory_model.model == MEMORY_MODEL_FLAT);
    REQUIRE(ast->the_memory_model.language_type == LANGUAGE_TYPE_PASCAL);
    REQUIRE(ast->the_memory_model.call_type == CALL_TYPE_STDCALL);
    REQUIRE(ast->the_memory_model.stack_option == STACK_OPTION_NEARSTACK);
  }

  TEST_CASE("NAME") {
    auto ast = masm::parse ("NAME pizda");
  }

  TEST_CASE("SEGORDER") {
    auto ast = masm::parse (".ALPHA");
    REQUIRE(ast->segments_order == SEGMENT_ORDER_ALPHA);
  }

  TEST_CASE("GROUP") {
    auto ast = masm::parse ("zopa GROUP pizda, pizda1");
    auto [s,e] = ast->groups.equal_range ("zopa");
    REQUIRE(s != e);
    REQUIRE(s->second == "pizda");
    s++;
    REQUIRE(s->second == "pizda1");
    s++;
    REQUIRE(s == e);
  }
}