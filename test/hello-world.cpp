#include "include/test.hpp"
#include "clixx/clixx.hpp"

TEST_MAIN(int argc, char** argv)
{
    clixx::CmdLine cmdl = {argc, argv};
    auto& options = cmdl.options();

    // when no arguments provided
    if (cmdl.argc() == 0) {
        // clixx::print() is a wrapper around fwrite()
        clixx::print("pass -h/--help to print hello world\n");
    }

    // for every short or long option
    for (const auto& opt : options)
    {
        if (opt.is("-h", "--help")) {
            clixx::print("Hello, World\n");
        }
    }
}
