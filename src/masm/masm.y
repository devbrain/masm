%include
{

#define MASM_PARSER_IN_LEMON
#include "masm.h"
#include "masm_parser_iface.h"
}

%token_type {struct masm_token*}
%extra_argument {struct masm_parser_ctx* ctx}

%token_prefix MASM_

%syntax_error {
        masm_parser_context_error (ctx, "Syntax error\n");
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
eol ::= COMMENT EOL.

directive ::= general_directive.
directives_list ::= directive.
directives_list ::= directive directives_list.

general_directive ::= model_directive. // | segorderdir | includelibdir | commentdir| groupdir | assumedir| structdir | typedefdir| externdir | publicdir | commdir | prototypedir| equdir | assdir | textdir| contextdir | optiondir |  radixdir | titledir | pagedir | listdir | crefdir | echodir| ifdir | errordir | includedir | macrodir | macrocall | macrorepeat | purgedir| macrowhile | macrofor | macroforc| aliasdir | recorddir | smartdir
general_directive ::= processor_dir.
general_directive ::= name_dir.

model_directive ::= MODEL memoption EOL.
model_directive ::= MODEL memoption COMMA model_opt_list EOL.

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

processor_dir ::= processor.
processor_dir ::= coprocessor.

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

name_dir ::= NAME ID EOL. {/* Ignored */}

%code
{

}

