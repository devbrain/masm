//
// Created by igor on 25/07/2022.
//
#include <doctest/doctest.h>
#include "test/lexer_utils.hh"
#include "masm.h"
#include "masm_lexer.h"
TEST_SUITE("test lexer") {
  TEST_CASE("EndOfInput") {
    REQUIRE(MASM_EOL == masm::test::scan_one_token (" "));
    REQUIRE(MASM_EOL == masm::test::scan_one_token (""));
  }

  TEST_CASE("EOL") {
    REQUIRE(MASM_EOL == masm::test::scan_one_token ("\n"));
  }

  TEST_CASE("Comment") {
    REQUIRE(MASM_COMMENT == masm::test::scan_one_token ("; Comment"));
    REQUIRE(MASM_COMMENT == masm::test::scan_one_token ("; Comment\n"));
  }

  TEST_CASE("MODEL") {
    REQUIRE(MASM_PASCAL    == masm::test::scan_one_token ("PASCAL"));
    REQUIRE(MASM_MODEL   == masm::test::scan_one_token (".MODEL"));
    REQUIRE(MASM_TINY    == masm::test::scan_one_token ("TINY"));
    REQUIRE(MASM_SMALL   == masm::test::scan_one_token ("SMALL"));
    REQUIRE(MASM_MEDIUM  == masm::test::scan_one_token ("MEDIUM"));
    REQUIRE(MASM_COMPACT == masm::test::scan_one_token ("COMPACT"));
    REQUIRE(MASM_LARGE   == masm::test::scan_one_token ("LARGE"));
    REQUIRE(MASM_HUGE    == masm::test::scan_one_token ("HUGE"));
    REQUIRE(MASM_FLAT    == masm::test::scan_one_token ("FLAT"));
    REQUIRE(MASM_FLAT    == masm::test::scan_one_token ("flat"));

  }

  TEST_CASE("PROC") {
    REQUIRE(MASM_PROC_386P   == masm::test::scan_one_token (".386p"));
    REQUIRE(MASM_COPROC_287   == masm::test::scan_one_token (".287"));
  }

  TEST_CASE("Keywords") {
    #include "test_keywords.h"
  }
}
