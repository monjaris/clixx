#pragma once
#include "core/defines.hpp"
#include "core/utils.hpp"
#include "Value.hpp"
#include "Option.hpp"
NAMESPACE_BEGIN(clixx)


// CLIXX
// basic definitions:
// values  -> `./prog hello foo 42`
// options -> `./prog --option value1 -o value1 value2`



class CLI
{
    struct M {
        int argc = -1;
        char** argv = nullptr;

        std::vector<Value> values;
        std::vector<Option> options;
        std::vector<std::string> orphans;
    } m;

public:
    class ParseResult {
        bool m_result = true;
    public:
        explicit ParseResult(bool success);
        bool failed();
    };

    CLI(int argc, char** argv);

    void printInfo(bool values = true, bool options = true, bool orphans = true);

    const char* const* argv() const;
    const std::vector<Value>& values() const;
    const std::vector<Option>& options() const;
    std::string repr() const;

    bool parse();
};


NAMESPACE_END()
