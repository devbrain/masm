//
// Created by igor on 27/07/2022.
//

#include "pretty_print.hh"
#include "masm.h"
#include "masm_lexer.h"

namespace masm {
  std::string token_type_to_string(int token_type) {
    switch (token_type) {
#include "masm_pp.h"
      case MASM_LEXER_ERROR:
        return "LEXER-ERROR";
      default:
        return "UNKNOWN";
    }
  }
}
