//
// Created by igor on 26/07/2022.
//

#include <cstring>
#include "test/lexer_utils.hh"
#include "masm_lexer.h"

namespace masm::test {
  int scan_one_token(const char* txt) {
    auto* lexer = masm_lexer_init (txt, strlen(txt));
    int rc = masm_lexer_scan (lexer);
    masm_lexer_done (lexer);
    return rc;
  }
}
