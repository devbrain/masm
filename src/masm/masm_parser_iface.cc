//
// Created by igor on 26/07/2022.
//

#include <stdexcept>
#include <cstdarg>
#include <cstring>
#include <list>
#include <string>
#include <iostream>

#include "masm_parser_iface.h"
#include "masm_lexer.h"

void masm_lexer_token_destructor(masm_token_ptr_t victim) {
  delete victim;
}

void masm_parser_context_error (struct masm_parser_ctx* ctx, const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf (stderr, fmt, arg);
  va_end(arg);
  fflush (stderr);
  ctx->syntax_error = true;
}

void masm_ast_set_memory_model(masm_parser_ctx* ctx, memory_model_t memory_model) {
  ctx->m_ast->the_memory_model.model = memory_model;
}
void masm_ast_set_call_type(masm_parser_ptr_t ctx, call_type_t call_type) {
  ctx->m_ast->the_memory_model.call_type = call_type;
}

void masm_ast_set_language(masm_parser_ptr_t ctx, language_type_t lang_type) {
  ctx->m_ast->the_memory_model.language_type = lang_type;
}

void masm_ast_set_stack_option(masm_parser_ptr_t ctx, stack_option_t stack_option) {
  ctx->m_ast->the_memory_model.stack_option = stack_option;
}

void masm_ast_set_processor_type(masm_parser_ptr_t ctx, processor_type_t processor_type) {
  ctx->m_ast->processor_type = processor_type;
}
void masm_ast_set_coprocessor_type(masm_parser_ptr_t ctx, coprocessor_type_t coprocessor_type) {
  ctx->m_ast->coprocessor_type = coprocessor_type;
}

void masm_ast_set_segments_order(masm_parser_ptr_t ctx, segments_order_t segments_order) {
  ctx->m_ast->segments_order = segments_order;
}
struct ast_string {
  explicit ast_string(const struct masm_token* tok)
  : value(tok->begin, tok->end - tok->begin) {}
  std::string value;
};

struct ast_string* ast_create_string(const struct masm_token* tok) {
  return new ast_string(tok);
}
void ast_destroy_string(struct ast_string* victim) {
  delete victim;
}

struct ast_string_list {
  ast_string_list() = default;

  explicit ast_string_list(std::string_view v) {
    data.emplace_back(v);
  }

  explicit ast_string_list(const ast_string* v) {
    data.emplace_back(v->value);
  }

  std::list <std::string> data;
};

struct ast_string_list* ast_string_list_create(const struct ast_string* tok) {
  return new ast_string_list(tok);
}
void ast_destroy_string_list(const struct ast_string_list* victim) {
  delete victim;
}

struct ast_string_list* ast_string_list_prepend(const ast_string* v, struct ast_string_list* lst) {
  lst->data.emplace_front (v->value);
  return lst;
}

struct ast_string_list* ast_string_list_append(struct ast_string_list* lst, const struct ast_string* v) {
  lst->data.emplace_back (v->value);
  return lst;
}

void ast_add_group_dir(masm_parser_ptr_t ctx, const ast_string* grp_name, const struct ast_string_list* lst) {
  for (const auto& grp : lst->data) {
    ctx->m_ast->groups.insert (std::make_pair(grp_name->value, grp));
  }
}


