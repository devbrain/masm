//
// Created by igor on 25/07/2022.
//
#include <iostream>
#include <cstring>
#include <mio/mmap.hpp>
#include <masm/masm-parser.hh>
#include "externc.h"
#include "masm_lexer.h"
#include "pretty_print.hh"
#include "masm_parser_iface.h"

dMASM_FUNC_DECL void* masm_parserAlloc (void* (* allocProc) (size_t));
dMASM_FUNC_DECL void  masm_parser (void*, int, struct masm_token*, struct masm_parser_ctx*);
dMASM_FUNC_DECL void  masm_parserFree (void*, void(* freeProc) (void*));
dMASM_FUNC_DECL void  masm_parserTrace (FILE* TraceFILE, const char* zTracePrompt);

namespace masm {
  namespace detail {
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

    std::unique_ptr<program> parse (const char* txt, std::size_t len, bool debug) {
      std::unique_ptr<masm_lexer, decltype (&masm_lexer_done)>
          lexer_ptr (masm_lexer_init (txt, len), masm_lexer_done);
      masm_parser_ctx ctx;
      auto_parser p;

      while (!masm_lexer_is_eof (lexer_ptr.get())) {
        auto rc = masm_lexer_scan (lexer_ptr.get ());
        if (debug) {
          auto tok_s = token_type_to_string (rc);
          std::cout << tok_s << std::endl;
        }
        if (rc == MASM_LEXER_ERROR) {
          throw std::runtime_error ("Lexer error");
        }
        else {
          auto* tok = new masm_token;
          masm_lexer_get_token (lexer_ptr.get (), tok);
          masm_parser (p.parser, rc, tok, &ctx);
          if (ctx.syntax_error) {
            std::cout << "Syntax error" << std::endl;
          }
        }
      }
      return std::move(ctx.m_ast);
    }
  }

  std::unique_ptr<program> parse (const std::filesystem::path& path, bool debug) {
    mio::mmap_source mmap(path.u8string().c_str());
    return detail::parse (mmap.data(), mmap.size(), debug);
  }

  std::unique_ptr<program> parse (const char* string, bool debug) {
    return detail::parse (string, strlen(string), debug);
  }

}