#include <print>
#include "include/test.h"
#include "clixx/clixx.hpp"


TEST_MAIN(int argc, char** argv)
{
    clixx::CmdLine cli = {argc, argv};

    for (const auto& opt : cli.options())
    {
        if (opt.is("-o", "--option")) {
            std::println("flag {} was passed", opt.name());
        }
    }


    std::println("{}", cli.repr());
    cli.printInfo();

    TEST_QUIT(TEST_QUIT_OK);
}
