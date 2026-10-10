#include "clixx/clixx.hpp"

int main(int argc, char** argv)
{
    clixx::CLI cli = {argc, argv};

    auto cli_parse = cli.parse();
    if (!cli_parse) cli.exit("Failed to parse command line");

    auto options = cli.options();

    for (const auto& opt : options) {
        if (opt.name() == "--option") {
            std::println("flag '--option' was passed");
        }
    }

    std::println("\n\n [[ Values ]] ");
    cli.printValues();
    std::println("\n\n [[ Options ]] ");
    cli.printOptions();
    std::println("\n\n [[ Orphans ]] ");
    cli.printOrphans();
}
