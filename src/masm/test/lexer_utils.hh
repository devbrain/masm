//
// Created by igor on 26/07/2022.
//

#ifndef SRC_MASM_TEST_LEXER_UTILS_HH
#define SRC_MASM_TEST_LEXER_UTILS_HH

#include <doctest/doctest.h>
#include "pretty_print.hh"

namespace masm::test {
  int scan_one_token(const char* txt);
}

#define ASSERT_LEXER(VALUE, TXT) REQUIRE(masm::token_type_to_string(VALUE) == masm::token_type_to_string(masm::test::scan_one_token (TXT)));

#endif //SRC_MASM_TEST_LEXER_UTILS_HH
