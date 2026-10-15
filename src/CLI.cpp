#include <print>
#include "clixx/CLI.hpp"
NAMESPACE_BEGIN(clixx)



// CLI::ParseResult implementation
CLI::ParseResult::ParseResult(bool success) 
    : m_result(success) 
{}


bool CLI::ParseResult::failed() {
    return m_result == false;
}


// CLI implementation
CLI::CLI(int argc, char** argv) {
    m.argc = argc;
    m.argv = argv;
    m.options.reserve(64);
    m.values.reserve(64);
}



const char* const* CLI::argv() const {
    return m.argv;
}


const std::vector<Value>& CLI::values() const {
    return m.values;
}


const std::vector<Option>& CLI::options() const {
    return m.options;
}


bool CLI::parse() {
    for (int i = 1; i < m.argc; ++i) {
        const char* const str = m.argv[i];

        if (Option::Valid(str)) {
            m.options.push_back(Option::Parse(m.argc, m.argv, i));
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



std::string CLI::repr() const {
    std::string repr;  repr.reserve(256);

    Option freestanding_values = {m.argv[0], m.values, true};
    repr += freestanding_values.repr();

    repr += ",\n";

    for (usize i=0;  i < m.options.size();  ++i) {
        repr.append( std::format("{}, ", m.options[i].repr()) );
    }

    return repr;
}


void CLI::printInfo(bool values, bool options, bool orphans) {
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


NAMESPACE_END()
