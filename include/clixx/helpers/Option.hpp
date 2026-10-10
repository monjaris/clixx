#pragma once
#include <vector>
#include "clixx/core/utils.hpp"
#include "Value.hpp"
NAMESPACE_BEGIN(clixx)


class Option
{
    std::string m_name;
    std::vector<Value> m_values;

public:
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wuninitialized"
    constexpr Option(const char* name, std::vector<Value> values)
        : m_values(values)
    {
        if (!Valid(name)) {
            m_name = {};
        } else {
            m_name = name;
        }
    }
#pragma GCC diagnostic pop

    static constexpr bool Valid(std::string str) {
        bool is = false;

        if (str.empty()) return false;
        char c = '-';

        if (str.starts_with("-") && not_at(c, str, 1) && str.size() == 2) {
            is = true;
        }

        if (str.starts_with("--") && not_at('-', str, 2) && str.size() > 2) {
            is = true;
        }

        return is;
    }

    const std::string& name() const {
        return m_name;
    }

    constexpr usize count() const {
        return m_values.size();
    }

    constexpr bool isFlag() const {
        return count() == 0;
    }

    std::optional<Value> get(usize i) const {
        if (i > count()-1) {
            return std::nullopt;
        } else {
            return m_values[i];
        }
    }

    auto values() const -> const std::vector<Value>& {
        return m_values;
    }

    Option& pushValue(Value arg) {
        m_values.push_back(arg);
        return *this;
    }


    static Option parse(int argc, char** argv, int& idx) {
        Option option = {argv[idx], {}};
        ++idx;

        for (;  idx < argc;  ++idx) {
            const auto& maybe_arg = argv[idx];

            // found next option, yield
            if (Option::Valid(maybe_arg)) {
                --idx; break;
            }

            option.pushValue(Value(maybe_arg));
        }

        return option;
    }
};


NAMESPACE_END()
