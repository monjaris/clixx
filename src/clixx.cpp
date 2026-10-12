#include "clixx/clixx.hpp"
NAMESPACE_BEGIN(clixx)


[[maybe_unused, noreturn]] void exitWith(const char* msg, int exit_code) {
    usize msg_len = util::cstrLen(msg);
    ::fwrite(msg, 1, msg_len, stdout);
    ::exit(exit_code);
}



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
    m.data = {argv, argv + argc};
    m.options.reserve(128);
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
        std::println("\n\n[[ Values ]]");
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


const std::vector<Value>& CLI::values() const {
    return m.values;
}


const std::vector<Option>& CLI::options() const {
    return m.options;
}


CLI::ParseResult CLI::parse() {
    for (int i = 1; i < m.argc; ++i) {
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


NAMESPACE_END()
