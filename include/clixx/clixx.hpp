#pragma once
#include "core/defines.hpp"
#include "core/utils.hpp"
#include "Value.hpp"
#include "Option.hpp"
#include "CLI.hpp"
NAMESPACE_BEGIN(clixx)


// exit with an error message and optionally custom exit code
[[maybe_unused, noreturn]]
void exitWith(const char* msg, int exit_code = 0);

// exit with an error message and optionally custom exit code
[[maybe_unused, noreturn]]
void abortWith(const char* msg);


// write a message to `stdout` (unbuffered)
[[maybe_unused]]
void print(const char* msg);

// write a message to `stderr` (unbuffered)
[[maybe_unused]]
void eprint(const char* msg);


NAMESPACE_END()
