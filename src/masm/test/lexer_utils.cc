//
// Created by igor on 26/07/2022.
//

#include "test/lexer_utils.hh"
#include "masm_lexer.h"
#include "istream.h"

namespace masm::test {
  int scan_one_token(const char* txt) {
    auto* istream = istream_create_from_string (txt);
    auto* lexer = masm_lexer_init (istream, 1024);
    int rc = masm_lexer_scan (lexer);
    masm_lexer_done (lexer);
    istream_close (istream);
    return rc;
  }
}
