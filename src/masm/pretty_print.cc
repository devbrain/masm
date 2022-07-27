//
// Created by igor on 27/07/2022.
//

#include "pretty_print.hh"
#include "masm.h"

namespace masm {
  const char* token_type_to_string(int token_type) {
    switch (token_type) {
#include "masm_pp.h"
      default:
        return "UNKNOWN";
    }
  }
}
