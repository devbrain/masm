//
// Created by igor on 26/07/2022.
//

#ifndef SRC_MASM_MASM_PARSER_IFACE_H
#define SRC_MASM_MASM_PARSER_IFACE_H

#include "externc.h"
#include <masm/masm-parser.hh>

#if defined(__GNUC__)
#define dMASM_ATTR_FORMAT __attribute__ ((format (printf, 2, 3)))
#else
#define dMASM_ATTR_FORMAT
#endif
struct masm_parser_ctx;
typedef struct masm_parser_ctx* masm_parser_ptr_t;

dMASM_FUNC_DECL void masm_parser_context_error(masm_parser_ptr_t ctx, const char *fmt, ...) dMASM_ATTR_FORMAT;

dMASM_FUNC_DECL void masm_ast_set_memory_model(masm_parser_ptr_t ctx, memory_model_t memory_model);
dMASM_FUNC_DECL void masm_ast_set_call_type(masm_parser_ptr_t ctx, call_type_t call_type);
dMASM_FUNC_DECL void masm_ast_set_language(masm_parser_ptr_t ctx, language_type_t lang_type);
dMASM_FUNC_DECL void masm_ast_set_stack_option(masm_parser_ptr_t ctx, stack_option_t stack_option);
dMASM_FUNC_DECL void masm_ast_set_processor_type(masm_parser_ptr_t ctx, processor_type_t processor_type);
dMASM_FUNC_DECL void masm_ast_set_coprocessor_type(masm_parser_ptr_t ctx, coprocessor_type_t coprocessor_type);

#if !defined(MASM_PARSER_IN_LEMON)

#endif

#endif //SRC_MASM_MASM_PARSER_IFACE_H
