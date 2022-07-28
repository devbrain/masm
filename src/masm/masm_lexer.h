//
// Created by igor on 24/07/2022.
//

#ifndef SRC_MASM_MASM_LEXER_H
#define SRC_MASM_MASM_LEXER_H

#include <stddef.h>
#include "externc.h"

#define MASM_LEXER_ERROR        -1
#define MASM_LEXER_END_OF_INPUT -2

struct masm_lexer;

dMASM_FUNC_DECL struct masm_lexer* masm_lexer_init(const char* text, size_t len);
dMASM_FUNC_DECL void masm_lexer_done(struct masm_lexer* victim);
dMASM_FUNC_DECL int masm_lexer_scan(struct masm_lexer* lexer);
dMASM_FUNC_DECL int masm_lexer_is_eof(struct masm_lexer* lexer);

struct masm_token {
  int line;
  const char* begin;
  const char* end;
};

dMASM_FUNC_DECL void masm_lexer_get_token(struct masm_lexer* lexer, struct masm_token* tok);

#endif //SRC_MASM_MASM_LEXER_H
