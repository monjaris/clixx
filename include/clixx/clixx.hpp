#pragma once
#include <print>
#include "core/defines.hpp"
#include "core/utils.hpp"
#include "helpers/Value.hpp"
#include "helpers/Option.hpp"
NAMESPACE_BEGIN(clixx)

// CLIXX
// basic definitions:
// values  -> `./prog hello foo 42`
// options -> `./prog --option value1 -o value1 value2`




[[maybe_unused, noreturn]]
void exitWith(const char* msg, int exit_code = 0) {
    std::println("{}", msg);
    ::exit(exit_code);
}



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
        explicit ParseResult(bool success): m_result(success) {}

        bool failed() { return m_result == false; }
    };


    CLI (int argc, char** argv)
    {
        m.argc = argc;
        m.argv = argv;

        m.data = {argv, argv + argc};

        m.options.reserve(128);
    }



    void printInfo(bool values = true, bool options = true, bool orphans = true) {
        std::println("\n\n----------- CLIXX CLI INFO -----------\n");

        auto longestNamedValue = [this]() {
            usize longest_length = 0;
            for (usize i=0;  i < m.values.size();  ++i) {
                longest_length = util::max(longest_length, m.values[i].str().size());
            }
            return longest_length;
        };

        auto longestNamedOption = [this]() {
            usize longest_length = 0;
            for (usize i=0;  i < m.options.size();  ++i) {
                longest_length = util::max(longest_length, m.options[i].name().size());
            }
            return longest_length;
        };


        if (values)
        {
            std::println("\n\n[[ Values ]]");

            std::string formatted;

            for (usize i = 0; i < m.values.size(); ++i) {
                auto& val = m.values[i];

                formatted += std::format("{}\n", val.repr(true));
            }

            std::println("{}", formatted);
        }

        if (options)
        {
            std::println("\n[[ Options ]]");
            std::string formatted;
            usize longest_name_sz = longestNamedOption();

            for (usize i=0;  i < m.options.size();  ++i) {
                auto& opt = m.options[i];

                std::string spaces(longest_name_sz - opt.name().size() + 2, ' ');
                formatted += std::format("{}:{}[ ", opt.name(), spaces);

                for (usize j=0;  j < opt.values().size();  ++j) {
                    formatted += std::format(
                        "{}", opt.values()[j].repr(j != opt.values().size())
                    );
                }

                formatted += " ]\n";
            }

            std::println("{}", formatted);
        }

        if (orphans)
        {
            std::println("\n[[ Orphans ]] ");

            for (const auto& orp : m.orphans) {
                std::print("{}; ", orp);
            }

            std::println();
        }
    }



    const std::vector<Value>& values() const {
        return m.values;
    }


    const std::vector<Option>& options() const {
        return m.options;
    }


    ParseResult parse() {
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

        return ParseResult(true);
    }
};




NAMESPACE_END()
