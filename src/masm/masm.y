%include
{

#define MASM_PARSER_IN_LEMON
#include "masm.h"
#include "masm_parser_iface.h"
}

%token_type {struct masm_token*}
%token_destructor {masm_lexer_token_destructor ($$);}
%extra_argument {struct masm_parser_ctx* ctx}

%token_prefix MASM_

%syntax_error {
        masm_parser_context_error (ctx, "Syntax error in parser\n");
        /*
        int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
        for (int i = 0; i < n; ++i) {
                int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
                if (a < YYNSTATE + YYNRULE) {
                        masm_parser_context_error (ctx, "possible token: %s\n", yyTokenName[i]);
                }
        }
        */
}

%parse_failure {
    masm_parser_context_error(ctx, "Parse failure\n");
}


%stack_overflow {
  masm_parser_context_error(ctx, "parser stack overflow");
}



%name masm_parser
%start_symbol program
// -------------------------------------------------------------
// MASM Grammar
// -------------------------------------------------------------

program ::= directives_list eol.
program ::= eol.

eol ::= EOL.
eol ::= END_OF_INPUT.

directive ::= general_directive.
directives_list ::= directive.
directives_list ::= directive directives_list.

general_directive ::= model_directive. // includelibdir | commentdir| assumedir| structdir | typedefdir| externdir | publicdir | commdir | prototypedir| equdir | assdir | textdir| contextdir | optiondir |  radixdir | titledir | pagedir |  crefdir | echodir| ifdir | errordir | includedir | macrodir | macrocall | macrorepeat | purgedir| macrowhile | macrofor | macroforc| aliasdir | recorddir | smartdir
general_directive ::= processor_dir.
general_directive ::= name_dir.
general_directive ::= seg_order_dir.
general_directive ::= list_dir.
general_directive ::= group_dir.

model_directive ::= MODEL memoption eol.
model_directive ::= MODEL memoption COMMA model_opt_list eol.

memoption ::= TINY.    {masm_ast_set_memory_model(ctx, MEMORY_MODEL_TINY);}
memoption ::= SMALL.   {masm_ast_set_memory_model(ctx, MEMORY_MODEL_SMALL);}
memoption ::= MEDIUM.  {masm_ast_set_memory_model(ctx, MEMORY_MODEL_MEDIUM);}
memoption ::= COMPACT. {masm_ast_set_memory_model(ctx, MEMORY_MODEL_COMPACT);}
memoption ::= LARGE.   {masm_ast_set_memory_model(ctx, MEMORY_MODEL_LARGE);}
memoption ::= HUGE.    {masm_ast_set_memory_model(ctx, MEMORY_MODEL_HUGE);}
memoption ::= FLAT.    {masm_ast_set_memory_model(ctx, MEMORY_MODEL_FLAT);}

model_opt_list ::= model_opt.
model_opt_list ::= model_opt COMMA model_opt_list.
model_opt      ::= language_type.
model_opt      ::= stack_option.
model_opt      ::= call_type.

language_type  ::= C.       {masm_ast_set_language(ctx, LANGUAGE_TYPE_C);}
language_type  ::= BASIC.   {masm_ast_set_language(ctx, LANGUAGE_TYPE_BASIC);}
language_type  ::= FORTRAN. {masm_ast_set_language(ctx, LANGUAGE_TYPE_FORTRAN);}
language_type  ::= PASCAL.  {masm_ast_set_language(ctx, LANGUAGE_TYPE_PASCAL);}
call_type  ::= SYSCALL.     {masm_ast_set_call_type(ctx, CALL_TYPE_SYSCALL);}
call_type  ::= STDCALL.     {masm_ast_set_call_type(ctx, CALL_TYPE_STDCALL);}
stack_option ::= NEARSTACK. {masm_ast_set_stack_option(ctx, STACK_OPTION_NEARSTACK);}
stack_option ::= FARSTACK.  {masm_ast_set_stack_option(ctx, STACK_OPTION_FARSTACK);}

processor_dir ::= processor eol.
processor_dir ::= coprocessor eol.

processor ::= PROC_386.  {masm_ast_set_processor_type (ctx, PROC_TYPE_386); }
processor ::= PROC_386P. {masm_ast_set_processor_type (ctx, PROC_TYPE_386P); }
processor ::= PROC_486.  {masm_ast_set_processor_type (ctx, PROC_TYPE_486); }
processor ::= PROC_486P. {masm_ast_set_processor_type (ctx, PROC_TYPE_486P); }
processor ::= PROC_586.  {masm_ast_set_processor_type (ctx, PROC_TYPE_586); }
processor ::= PROC_586P. {masm_ast_set_processor_type (ctx, PROC_TYPE_586P); }
processor ::= PROC_686.  {masm_ast_set_processor_type (ctx, PROC_TYPE_686); }
processor ::= PROC_686P. {masm_ast_set_processor_type (ctx, PROC_TYPE_686P); }

coprocessor ::= COPROC_8087. {masm_ast_set_coprocessor_type (ctx, COPROC_TYPE_8087); }
coprocessor ::= COPROC_287.  {masm_ast_set_coprocessor_type (ctx, COPROC_TYPE_287); }
coprocessor ::= COPROC_387.  {masm_ast_set_coprocessor_type (ctx, COPROC_TYPE_387); }
coprocessor ::= COPROC_NO87. {masm_ast_set_coprocessor_type (ctx, COPROC_TYPE_NO87); }

name_dir ::= NAME ID eol. {/* Ignored */}

seg_order_dir ::= ALPHA eol.   {masm_ast_set_segments_order(ctx, SEGMENT_ORDER_ALPHA); }
seg_order_dir ::= SEQ eol.     {masm_ast_set_segments_order(ctx, SEGMENT_ORDER_SEQ); }
seg_order_dir ::= DOSSEG eol.  {masm_ast_set_segments_order(ctx, SEGMENT_ORDER_DOS); }
seg_order_dir ::= PDOSSEG eol. {masm_ast_set_segments_order(ctx, SEGMENT_ORDER_DOS); }

list_dir ::= list_dir_opt eol. {/* Ignored */}
list_dir_opt ::= LIST.
list_dir_opt ::= NOLIST.
list_dir_opt ::= XLIST.
list_dir_opt ::= LISTALL.
list_dir_opt ::= LISTIF.
list_dir_opt ::= LFCOND.
list_dir_opt ::= NOLISTIF.
list_dir_opt ::= SFCOND.
list_dir_opt ::= TFCOND.
list_dir_opt ::= LISTMACROALL.
list_dir_opt ::= LALL.
list_dir_opt ::= NOLISTMACRO.
list_dir_opt ::= SALL.
list_dir_opt ::= LISTMACRO.
list_dir_opt ::= XALL.

%type        seg_id_list {struct ast_string_list*}
%destructor  seg_id_list {ast_destroy_string_list($$);}
%type        seg_id      {struct ast_string*}
%destructor  seg_id      {ast_destroy_string($$);}
%type        group_id    {struct ast_string*}
%destructor  group_id    {ast_destroy_string($$);}

group_dir ::= group_id(A) GROUP seg_id_list(B).    {ast_add_group_dir(ctx, A, B);}
group_id(Y) ::= ID(A).                             {Y = ast_create_string(A);}
seg_id_list(Y) ::= seg_id(A).                      {Y = ast_string_list_create(A);}
seg_id_list(Y) ::= seg_id_list(B) COMMA seg_id(A). {Y = ast_string_list_append(B, A); }
seg_id(Y) ::= ID(A).                               {Y = ast_create_string(A);}

%code
{

}

