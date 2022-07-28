//
// Created by igor on 25/07/2022.
//
#include <doctest/doctest.h>
#include "test/lexer_utils.hh"
#include "masm.h"

TEST_SUITE("test lexer") {
  TEST_CASE("EndOfInput") {
    ASSERT_LEXER(MASM_ALPHA, ".ALPHA");
    ASSERT_LEXER(MASM_END_OF_INPUT," ");
    ASSERT_LEXER(MASM_END_OF_INPUT,"");
  }

  TEST_CASE("EOL") {
    ASSERT_LEXER(MASM_EOL,"\n");
  }

  TEST_CASE("MODEL") {
    ASSERT_LEXER(MASM_PASCAL  ,"PASCAL");
    ASSERT_LEXER(MASM_MODEL   ,".MODEL");
    ASSERT_LEXER(MASM_TINY    ,"TINY");
    ASSERT_LEXER(MASM_SMALL   ,"SMALL");
    ASSERT_LEXER(MASM_MEDIUM  ,"MEDIUM");
    ASSERT_LEXER(MASM_COMPACT ,"COMPACT");
    ASSERT_LEXER(MASM_LARGE   ,"LARGE");
    ASSERT_LEXER(MASM_HUGE    ,"HUGE");
    ASSERT_LEXER(MASM_FLAT    ,"FLAT");
    ASSERT_LEXER(MASM_FLAT    ,"flat");

  }

  TEST_CASE("PROC") {
    ASSERT_LEXER(MASM_PROC_386P   , ".386p");
    ASSERT_LEXER(MASM_COPROC_287  , ".287");
  }

  TEST_CASE("Keywords") {
    #include "test_keywords.h"
  }
}
