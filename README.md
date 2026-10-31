# clixx - Simple command-line parsing library

- no exceptions, no rtti
- statically compiled
- fast, simple, easy-to-prototype
- aiming for cross-platform ecosystem


# Setup
NOTE: You need to have [xmake](https://xmake.io) to build clixx

```bash
git clone https://github.com/monjaris/clixx && cd clixx
xmake
```

# Hello-World
```cpp
#include <clixx/clixx.hpp>

int main(int argc, char** argv)
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
```


# Why clixx?
Command line parsing in C++ had many options throughout the history.. \
CLI11, cxxopts, Argh!, argparse, etc.

Event though most of them are good for many kind of projects, they have problems that we should look at:
- Forced usage of exceptions(CLI11)
- Huge compile times(CLI11, cxxopts)
- Subcommands are only available in declarative parsing(argh!)

clixx, fixes most of these problems. You might ask, we can even have non-declerative subcommands? - yes: [test/subcmd.cpp](test/subcmd.cpp:11:54)
That is actually what most small C/C++ projects have always been doing.

Another fact is that clixx can be used as a cl-parsing backend library for another preferably declerative cl-parsing library.



# License
This project is MIT licensed: [LICENSE.txt](LICENSE.txt)
