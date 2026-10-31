#include <print>
#include "include/test.h"
#include "clixx/clixx.hpp"


TEST_MAIN(int argc, char** argv)
{
    clixx::CmdLine cmdl = {argc, argv};

    if (cmdl.argc() > 1) {
        clixx::CmdLine sc_run = {argc - 1, argv + 1};
        std::println("{}", sc_run.repr());
    }
    else {
        std::println("{}: pass 'run' to see subcommand metadata", cmdl.argv()[0]);
    }

    TEST_QUIT(TEST_QUIT_OK);
}
