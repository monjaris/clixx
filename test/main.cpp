#include "clixx/clixx.hpp"

int main(int argc, char** argv)
{
    clixx::CLI cli = {argc, argv};

    auto cli_parse = cli.parse();
    if (!cli_parse) cli.exit("Failed to parse");

    auto options = cli.options();

    for (const auto& opt : options) {
        if (opt.name() == "--ala") {
            std::println("rfe");
        }
    }

    std::println("\n\n [[ Subcommands ]] ");
    cli.printSubcmds();
    std::println("\n\n [[ Options ]] ");
    cli.printOptions();
    std::println("\n\n [[ Orphans ]] ");
    cli.printOrphans();
}
