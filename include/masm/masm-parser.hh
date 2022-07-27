//
// Created by igor on 25/07/2022.
//

#ifndef INCLUDE_MASM_MASM_PARSER_HH
#define INCLUDE_MASM_MASM_PARSER_HH

#include <masm-parser_export.h>

typedef enum {
  MEMORY_MODEL_TINY,
  MEMORY_MODEL_SMALL,
  MEMORY_MODEL_MEDIUM,
  MEMORY_MODEL_COMPACT,
  MEMORY_MODEL_LARGE,
  MEMORY_MODEL_HUGE,
  MEMORY_MODEL_FLAT
} memory_model_t;

typedef enum  {
  LANGUAGE_TYPE_C,
  LANGUAGE_TYPE_PASCAL,
  LANGUAGE_TYPE_BASIC,
  LANGUAGE_TYPE_FORTRAN
} language_type_t;

typedef enum {
  CALL_TYPE_STDCALL,
  CALL_TYPE_SYSCALL
} call_type_t;

typedef enum {
  STACK_OPTION_NEARSTACK,
  STACK_OPTION_FARSTACK,
} stack_option_t;

typedef enum {
  PROC_TYPE_386,
  PROC_TYPE_386P,
  PROC_TYPE_486,
  PROC_TYPE_486P,
  PROC_TYPE_586,
  PROC_TYPE_586P,
  PROC_TYPE_686,
  PROC_TYPE_686P
} processor_type_t;

typedef enum {
  COPROC_TYPE_8087,
  COPROC_TYPE_287,
  COPROC_TYPE_387,
  COPROC_TYPE_NO87,
} coprocessor_type_t;

#if !defined(MASM_PARSER_IN_LEMON)

#include <memory>
#include <filesystem>

namespace masm {
  struct MASM_PARSER_EXPORT memory_model {
    memory_model_t model;
    language_type_t language_type;
    call_type_t call_type;
    stack_option_t stack_option;
  };

  struct MASM_PARSER_EXPORT program {
    memory_model the_memory_model;
    processor_type_t processor_type;
    coprocessor_type_t coprocessor_type;
  };

  MASM_PARSER_EXPORT std::unique_ptr<program> parse(const std::filesystem::path& path, bool debug = false);
  MASM_PARSER_EXPORT std::unique_ptr<program> parse(const char* string, bool debug = false);

}
#endif

#endif //INCLUDE_MASM_MASM_PARSER_HH
