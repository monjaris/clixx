#include <print>
#include "include/test.h"
#include "clixx/clixx.hpp"


TEST_MAIN(int argc, char** argv)
{
    clixx::CmdLine cmdl = {argc, argv};

    if (cmdl.argc() > 1) {
        clixx::CmdLine subcmd = {argc - 1, argv + 1};
        std::println("{}", subcmd.repr());
    }
    else {
        std::println("{}: argc should be more than 1 to process subcommands", cmdl.argv()[0]);
    }

    TEST_QUIT(TEST_QUIT_OK);
}
