//
// Created by igor on 25/07/2022.
//

#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest/doctest.h>

DOCTEST_SYMBOL_IMPORT void from_dll ();

int main (int argc, char** argv) {
  // force the use of a symbol from the dll so tests from it get registered
  from_dll ();

  doctest::Context context (argc, argv);
  int res = context.run ();

  if (context.shouldExit ()) // important - query flags (and --exit) rely on the user doing this
    return res;          // propagate the result of the tests

  int client_stuff_return_code = 0;
  // your program - if the testing framework is integrated in your production code

  return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
