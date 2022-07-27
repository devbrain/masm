//
// Created by igor on 24/07/2022.
//

#ifndef SRC_MASM_EXTERNC_H
#define SRC_MASM_EXTERNC_H

#if defined(__cplusplus)
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define dMASM_FUNC_DECL EXTERN_C

#endif //SRC_MASM_EXTERNC_H
