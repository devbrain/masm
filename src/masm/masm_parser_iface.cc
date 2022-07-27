//
// Created by igor on 26/07/2022.
//

#include <stdexcept>
#include <cstdarg>
#include <iostream>

#include "masm_parser_iface.h"
#include "masm_lexer.h"
#include "pretty_print.hh"

// Forward declarations. These functions are generated by lemon

dMASM_FUNC_DECL void* masm_parserAlloc (void* (* allocProc) (size_t));
dMASM_FUNC_DECL void  masm_parser (void*, int, struct masm_token*, struct masm_parser_ctx*);
dMASM_FUNC_DECL void  masm_parserFree (void*, void(* freeProc) (void*));
dMASM_FUNC_DECL void  masm_parserTrace (FILE* TraceFILE, const char* zTracePrompt);

struct masm_parser_ctx {
  masm_parser_ctx ()
  : m_ast(std::make_unique<masm::program>()),
  syntax_error(false) {
  }
  std::unique_ptr<masm::program> m_ast;
  bool syntax_error;
};

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

namespace masm {
  namespace detail {
    using istream_ptr_t = std::unique_ptr<istream, decltype (&istream_close)>;

    struct file_closer {
      FILE* file;

      explicit file_closer (FILE* f)
          : file (f) {
      }

      ~file_closer () {
        fclose (file);
      }
    };

    struct auto_parser {
      void* parser;

      auto_parser ()
          : parser (masm_parserAlloc (malloc)) {
        if (!parser) {
          throw std::runtime_error ("Failed to allocate parser");
        }
      }

      ~auto_parser () {
        masm_parserFree (parser, free);
      }
    };

    std::unique_ptr<program> parse (istream* is, bool debug) {
      std::unique_ptr<masm_lexer, decltype (&masm_lexer_done)> lexer_ptr (masm_lexer_init (is, 1024), masm_lexer_done);
      masm_parser_ctx ctx;
      auto_parser p;

      while (!masm_lexer_is_eof (lexer_ptr.get())) {
        auto rc = masm_lexer_scan (lexer_ptr.get ());
        if (debug) {
          const char* tok_s = token_type_to_string (rc);
          std::cout << tok_s << std::endl;
        }
        if (rc == MASM_LEXER_ERROR) {
          throw std::runtime_error ("Lexer error");
        }
        else {
          masm_token tok{};
          masm_lexer_get_token (lexer_ptr.get (), &tok);
          masm_parser (p.parser, rc, &tok, &ctx);
          if (ctx.syntax_error) {
            std::cout << "Syntax error" << std::endl;
          }
        }
      }
      return std::move(ctx.m_ast);
    }

  }

  std::unique_ptr<program> parse (const std::filesystem::path& path, bool debug) {
    FILE* file = fopen (path.u8string ().c_str (), "rb");
    if (!file) {
      throw std::runtime_error (std::string ("Can not open file ") + path.c_str ());
    }
    detail::file_closer auto_file (file);
    detail::istream_ptr_t istream_ptr (istream_create_from_file (file), istream_close);
    return detail::parse (istream_ptr.get (), debug);
  }

  std::unique_ptr<program> parse (const char* string, bool debug) {
    detail::istream_ptr_t istream_ptr (istream_create_from_string (string), istream_close);
    return detail::parse (istream_ptr.get (), debug);
  }

}