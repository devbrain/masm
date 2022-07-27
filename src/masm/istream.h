//
// Created by igor on 25/07/2022.
//

#ifndef SRC_MASM_ISTREAM_H
#define SRC_MASM_ISTREAM_H

#include <stddef.h>
#include <stdio.h>
#include "externc.h"

struct istream {
  void* opaque;

  void (*destructor) (void*);
  int  (*is_eof)(void*);
  ssize_t (*read)(void*, void*, size_t);
};

dMASM_FUNC_DECL struct istream* istream_create_from_file(FILE* input);
dMASM_FUNC_DECL struct istream* istream_create_from_string(const char* input);
dMASM_FUNC_DECL void istream_close(struct istream* victim);
dMASM_FUNC_DECL int istream_eof(struct istream* is);
dMASM_FUNC_DECL ssize_t istream_read(struct istream* is, void* dest, size_t size);


#endif //SRC_MASM_ISTREAM_H
