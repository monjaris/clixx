#include <print>
#include "include/test.hpp"
#include "clixx/clixx.hpp"


TEST_MAIN(int argc, char** argv)
{
    clixx::CmdLine cmdl = {argc, argv};
    cmdl.parse();

    if (cmdl.argc() > 1) {
        clixx::CmdLine sc_run = {argc - 1, argv + 1};
        sc_run.parse();

        std::println("{}", sc_run.repr());
    }

    TEST_QUIT(TEST_QUIT_OK);
}
