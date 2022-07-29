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
struct masm_token;
typedef struct masm_parser_ctx* masm_parser_ptr_t;
typedef struct masm_token* masm_token_ptr_t;

dMASM_FUNC_DECL void masm_lexer_token_destructor(masm_token_ptr_t victim);
dMASM_FUNC_DECL void masm_parser_context_error(masm_parser_ptr_t ctx, const char *fmt, ...) dMASM_ATTR_FORMAT;

dMASM_FUNC_DECL void masm_ast_set_memory_model(masm_parser_ptr_t ctx, memory_model_t memory_model);
dMASM_FUNC_DECL void masm_ast_set_call_type(masm_parser_ptr_t ctx, call_type_t call_type);
dMASM_FUNC_DECL void masm_ast_set_language(masm_parser_ptr_t ctx, language_type_t lang_type);
dMASM_FUNC_DECL void masm_ast_set_stack_option(masm_parser_ptr_t ctx, stack_option_t stack_option);
dMASM_FUNC_DECL void masm_ast_set_processor_type(masm_parser_ptr_t ctx, processor_type_t processor_type);
dMASM_FUNC_DECL void masm_ast_set_coprocessor_type(masm_parser_ptr_t ctx, coprocessor_type_t coprocessor_type);
dMASM_FUNC_DECL void masm_ast_set_segments_order(masm_parser_ptr_t ctx, segments_order_t segments_order);

struct ast_string_list;
struct ast_string;
dMASM_FUNC_DECL struct ast_string* ast_create_string(const struct masm_token* tok);
dMASM_FUNC_DECL void ast_destroy_string(struct ast_string* victim);

dMASM_FUNC_DECL struct ast_string_list* ast_string_list_create(const struct ast_string* tok);
dMASM_FUNC_DECL void ast_destroy_string_list(const struct ast_string_list* victim);
dMASM_FUNC_DECL struct ast_string_list* ast_string_list_prepend(const struct ast_string* v, struct ast_string_list* lst);
dMASM_FUNC_DECL struct ast_string_list* ast_string_list_append(struct ast_string_list* lst, const struct ast_string* v);
dMASM_FUNC_DECL void ast_add_group_dir(masm_parser_ptr_t ctx, const struct ast_string* name, const struct ast_string_list* lst);
#if !defined(MASM_PARSER_IN_LEMON)
#include <memory>

struct masm_parser_ctx {
  masm_parser_ctx ()
      : m_ast(std::make_unique<masm::program>()),
        syntax_error(false) {
  }
  std::unique_ptr<masm::program> m_ast;
  bool syntax_error;
};
#endif
#endif //SRC_MASM_MASM_PARSER_IFACE_H
