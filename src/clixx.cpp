#include "clixx/clixx.hpp"
CLIIXX_NAMESPACE_BEGIN()


// CLIXX
// basic definitions:
// values  -> `./prog hello foo 42`
// options -> `./prog --option value1 -o value1 value2`



void printExit(const char* msg, int exit_code) {
    print(msg);
    ::exit(exit_code);
}

void printAbort(const char* msg) {
    eprint(msg);
    ::abort();
}


void print(const char* msg) {
    ::fwrite(msg, sizeof(char), util::cstrLen(msg), stdout);
}


// write to `stderr` (unbuffered)
void eprint(const char* err_msg) {
    ::fwrite(err_msg, sizeof(char), util::cstrLen(err_msg), stderr);
}


CLIXX_NAMESPACE_END()
