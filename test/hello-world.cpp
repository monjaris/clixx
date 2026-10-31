#include "clixx/clixx.hpp"

int main(int argc, char** argv)
{
    clixx::CmdLine cmdl = {argc, argv};
    auto& values = cmdl.values();
    auto& options = cmdl.options();

    // for every short or long option
    for (const auto& opt : options)
    {
        if (opt.is("-h", "--help")) {
            clixx::print("Hello, World\n");  // clixx::print() is a wrapper around fwrite()
        }
    }
}
