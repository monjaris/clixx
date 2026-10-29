#pragma once
#include "core/defines.hpp"
#include "core/utils.hpp"
#include "Value.hpp"
#include "Option.hpp"
CLIXX_NAMESPACE_BEGIN()


// CLIXX
// basic definitions:
// values  -> `./prog hello foo 42`
// options -> `./prog --option value1 -o value1 value2`



class CmdLine
{
    struct M {
        usize argc = -1;
        char** argv = nullptr;

        std::vector<Value> values;
        std::vector<Option> options;
        std::vector<std::string> orphans;
    } m;

public:
    CmdLine(int argc, char** argv);
    CmdLine(usize argc, char** argv);

    void printInfo(bool values = true, bool options = true, bool orphans = true);

    usize argc() const;
    const char* const* argv() const;
    char** argv();

    const std::vector<Value>& values() const;
    const std::vector<Option>& options() const;
    [[nodiscard]] std::string repr() const;

    bool parse();
};


CLIXX_NAMESPACE_END()
