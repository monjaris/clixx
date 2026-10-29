#include <print>
#include "clixx/CmdLine.hpp"
CLIXX_NAMESPACE_BEGIN()


CmdLine::CmdLine(usize argc, char** argv) {
    m.argc = static_cast<usize>(argc);
    m.argv = argv;
    m.options.reserve(64);
    m.values.reserve(64);
}

// argc is deliberatly promoted from int to size type
CmdLine::CmdLine(int argc, char** argv) {
    m.argc = static_cast<usize>(argc);
    m.argv = argv;
    m.options.reserve(64);
    m.values.reserve(64);
}



usize CmdLine::argc() const {
    return m.argc;
}


const char* const* CmdLine::argv() const {
    return m.argv;
}

// overload
char** CmdLine::argv() {
    return m.argv;
}


const std::vector<Value>& CmdLine::values() const {
    return m.values;
}


const std::vector<Option>& CmdLine::options() const {
    return m.options;
}


bool CmdLine::parse() {
    for (usize i = 1; i < m.argc; ++i) {
        const char* const str = m.argv[i];

        if (Option::Valid(str)) {
            m.options.push_back(Option::Parse(m.argc, m.argv, i));
        }
        else if (true) {
            m.values.push_back(Value(str));
        }
        else {
            m.orphans.push_back(str);
        }
    }

    return true;
}



std::string CmdLine::repr() const {
    std::string repr;  repr.reserve(256);

    Option freestanding_values = {m.argv[0], m.values, true};
    repr += freestanding_values.repr();

    repr += ",\n";

    for (usize i=0;  i < m.options.size();  ++i) {
        repr.append( std::format("{}, ", m.options[i].repr()) );
    }

    return repr;
}


void CmdLine::printInfo(bool values, bool options, bool orphans) {
    std::println("\n\n----------- CLIXX CLI INFO -----------\n");

    auto longestNamedOption = [this]() {
        usize longest_length = 0;
        for (usize i = 0; i < m.options.size(); ++i) {
            longest_length = util::max(longest_length, m.options[i].name().size());
        }
        return longest_length;
    };

    if (values) {
        std::println("\n[[ Values ]]");
        std::string formatted;

        for (usize i = 0; i < m.values.size(); ++i) {
            auto& val = m.values[i];
            formatted += std::format("{}\n", val.repr(true));
        }

        std::println("{}", formatted);
    }

    if (options) {
        std::println("\n[[ Options ]]");
        std::string formatted;
        usize longest_name_sz = longestNamedOption();

        for (usize i = 0; i < m.options.size(); ++i) {
            auto& opt = m.options[i];

            std::string spaces(longest_name_sz - opt.name().size() + 2, ' ');
            formatted += std::format("{}:{}[ ", opt.name(), spaces);

            for (usize j = 0; j < opt.values().size(); ++j) {
                formatted += std::format(
                    "{}", opt.values()[j].repr(j != opt.values().size())
                );
            }

            formatted += " ]\n";
        }

        std::println("{}", formatted);
    }

    if (orphans) {
        std::println("\n[[ Orphans ]] ");

        for (const auto& orp : m.orphans) {
            std::print("{}; ", orp);
        }

        std::println();
    }
}


CLIXX_NAMESPACE_END()
