#pragma once
#include <string>
#include <vector>
#include <print>
#include "Option.hpp"
#include "Subcmd.hpp"
NAMESPACE_BEGIN(clixx)



// CLIXX
// basic definitions:
// options     -> `./prog --option x` || `./prog -o x y z` || `./prog --opt`
// subcommands -> `./prog subcmd --option x -o x y z`



class CLI
{
    struct M {
        char** argv = nullptr;
        int argc = -1;
        std::vector<std::string> data;
        std::vector<Option> options;
        std::vector<Subcmd> subcommands;
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


    void printOptions() {
        for (const auto& option : m.options) {
            std::println("{}", option.fmt());
        }
    }
    void printSubcmds() {
        for (const auto& subcmd : m.subcommands) {
            std::println("{}", subcmd.fmt());
        }
    }
    void printOrphans() {
        for (const auto& orp : m.orphans) {
            std::print("{}; ", orp);
        }
        std::println();
    }


    const std::vector<Option>& options() {
        return m.options;
    }

    const std::vector<Subcmd>& subcommands() {
        return m.subcommands;
    }


    Option parseOption(char** argv, int& idx) {
        Option option = {argv[idx], {}};
        ++idx;

        for (;  idx < m.argc;  ++idx) {
            const auto& maybe_arg = m.data[idx];

            // found next option, yield
            if (Option::Valid(maybe_arg)) {
                --idx; break;
            }

            option.pushValue(Value(maybe_arg));
        }

        return option;
    }

    Subcmd parseSubcmd(char** argv, int& idx) {
        Subcmd subcmd = {argv[idx], {}};
        ++idx;

        for (;  idx < m.argc;  ++idx) {
            const auto& maybe_arg = m.data[idx];

            // found next subcommand, yield
            if (Subcmd::Valid(maybe_arg)) {
                --idx; break;
            }
        }

        return subcmd;
    }


    [[nodiscard]] bool parse() {
        for (int i=1;  i < m.argc;  ++i) {
            auto& str = m.data[i];

            bool subcommand_found = false;

            if (Subcmd::Valid(str)) {
                subcommand_found = true;
                m.subcommands.push_back(parseSubcmd(m.argv, i));
            }
            else if (Option::Valid(str) && !subcommand_found) {
                m.options.push_back(parseOption(m.argv, i));
            }
            else {
                m.orphans.push_back(str);
            }
        }

        return true;
    }
};



NAMESPACE_END()
