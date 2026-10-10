#pragma once
#include <print>
#include "core/defines.hpp"
#include "core/utils.hpp"
#include "helpers/Value.hpp"
#include "helpers/Option.hpp"
NAMESPACE_BEGIN(clixx)

// CLIXX
// basic definitions:
// values  -> `./prog hello 42`
// options -> `./prog --option x -a 1 2 3`



class CLI
{
    struct M {
        int argc = -1;
        char** argv = nullptr;
        std::vector<std::string> data;

        std::vector<Value> values;
        std::vector<Option> options;
        std::vector<std::string> orphans;
    } m;

public:

    class ParseResult {
        bool m_result;
    public:
        ParseResult(bool success): m_result(success) {}

        bool operator! () {
            return m_result;
        }
    };


    CLI (int argc, char** argv)
    {
        m.argc = argc;
        m.argv = argv;

        m.data = {argv, argv + argc};

        m.options.reserve(128);
    }

    [[noreturn]] void exit(const char* msg, int exit_code = 0) {
        std::println("{}", msg);
        ::exit(exit_code);
    }


    void printValues() {
        for (const auto& val : m.values) {
            std::println("{}", val.repr());
        }
    }

    void printOptions() {
        for (const auto& option : m.options) {
            std::print("{}: ", option.name());

            for (usize i=0;  i < option.values().size();  ++i) {
                std::print(
                    "{}", option.values()[i].repr(i != option.values().size()-1)
                );
            }

            std::println();
        }
    }

    void printOrphans() {
        for (const auto& orp : m.orphans) {
            std::println("{}", orp);
        }
    }


    const std::vector<Option>& options() {
        return m.options;
    }


    [[nodiscard]] bool parse() {
        for (int i=1;  i < m.argc;  ++i) {
            std::string& str = m.data[i];

            if (Option::Valid(str)) {
                m.options.push_back(Option::parse(m.argc, m.argv, i));
            }
            else if (Value::Valid(str)) {
                m.values.push_back(Value(str));
            }
            else {
                m.orphans.push_back(str);
            }
        }

        return true;
    }
};




NAMESPACE_END()
