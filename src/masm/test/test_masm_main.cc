//
// Created by igor on 25/07/2022.
//

#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

DOCTEST_SYMBOL_EXPORT void from_dll ();   // to silence "-Wmissing-declarations" with GCC
DOCTEST_SYMBOL_EXPORT void from_dll () {} // force the creation of a .lib file with
