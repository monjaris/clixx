#include "clixx/clixx.hpp"

int main(int argc, char** argv)
{
    clixx::CLI cli = {argc, argv};

    if (cli.parse().failed()) {
        clixx::exitWith("Failed to parse command line");
    }

    for (const auto& opt : cli.options())
    {
        if (opt.is("-o", "--option")) {
            std::println("flag {} was passed", opt.name());
        }
    }

    cli.printInfo();
}
