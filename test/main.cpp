#include <print>
#include "include/test.hpp"
#include "clixx/clixx.hpp"

TEST_MAIN(int argc, char** argv)
{
    clixx::CLI cli = {argc, argv};

    if (!cli.parse()) {
        clixx::exitWith("Failed to parse command line");
    }

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
