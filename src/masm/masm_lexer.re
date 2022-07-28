#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "masm_lexer.h"
#include "masm.h"


struct masm_lexer {
  const char* top;
  const char* cur;
  const char* ptr;
  const char* pos;
  const char* end;
  int eof;
  int line;
};


struct masm_lexer* masm_lexer_init(const char* text, size_t len) {
  struct masm_lexer* lexer = (struct masm_lexer*)malloc(sizeof(struct masm_lexer));
  lexer->top  = text;
  lexer->cur  = text;
  lexer->pos  = text;
  lexer->end  = text + len;
  lexer->line = 1;
  lexer->eof = 0;
  return lexer;
}

void masm_lexer_done(struct masm_lexer* victim) {
  if (victim) {
    free(victim);
  }
}

int masm_lexer_is_eof(struct masm_lexer* lexer) {
  return lexer->eof;
}


void masm_lexer_get_token(struct masm_lexer* lexer, struct masm_token* tok) {
  if (lexer == NULL || tok == NULL) {
    return;
  }
  tok->line  = lexer->line;
  tok->begin = lexer->top;
  tok->end   = lexer->cur;
}

static int icase_cmp_token(struct masm_lexer* lexer, const char* name) {
  size_t tok_size = lexer->cur - lexer->top;
  int i = 0;
  if (strlen(name) != tok_size) {
    return 0;
  }
  for (i=0; i<tok_size; i++) {
    if (tolower(name[i]) != tolower (lexer->top[i])) {
      return 0;
    }
  }
  return 1;
}

#define dTEST_MACRO(NAME) icase_cmp_token(lexer, NAME)

static int handle_identifier(struct masm_lexer* lexer) {
#include "lexer_kw_keywords.h"
  return MASM_ID;
}

int masm_lexer_scan(struct masm_lexer* lexer) {
#define   YYCTYPE     char
#define   YYCURSOR    lexer->cur
#define   YYMARKER    lexer->ptr

  regular:
  if (lexer->cur >= lexer->end)
  {
    lexer->cur = lexer->end;
    lexer->eof = 1;
    return MASM_END_OF_INPUT;
  }
  lexer->top = lexer->cur;

  /*!re2c
    re2c:yyfill:enable = 0;

    ALPHANUMS = [a-zA-Z0-9]+;
    whitespace = [ \t\v\f]+;
    dig = [0-9];
    let = [a-zA-Z];
    D        = [0-9] ;
    L        = [a-zA-Z_] ;
    identifier = L ( L | D )*;
    any = [^];
    ESC      = [\\] ;
    SQ       = ['] ;
    DQ       = ["] ;

    STRING1  = SQ ( any \ SQ \ ESC | ESC any )* SQ ;
    STRING2  = DQ ( any \ DQ \ ESC | ESC any )* DQ ;
   */

  /*!re2c

  ";" {goto comment;}

  whitespace        { goto regular; }
  !include "lexer_keywords.h";


  identifier        { return handle_identifier(lexer); }
  //STRING1           { return create_token (lexer, PROTOGEN_TOKEN_STRING , tok); }
  //STRING2           { return create_token (lexer, PROTOGEN_TOKEN_STRING , tok); }

  "\r\n"|"\n" {
  lexer->pos = lexer->cur;
  lexer->line++;
  return MASM_EOL;
  }

  any              {return MASM_LEXER_ERROR;}
  */

  comment:
  if (lexer->cur >= lexer->end)
  {
    lexer->cur = lexer->end;
    return MASM_END_OF_INPUT;
  }
  /*!re2c
    "\n"          { lexer->line++; goto regular; }
    any           { goto comment; }
  */
  lexer->eof = 1;
  return MASM_END_OF_INPUT;
}