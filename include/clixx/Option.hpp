#pragma once
#include <vector>
#include <format>
#include "core/defines.hpp"
#include "core/utils.hpp"
#include "Value.hpp"
NAMESPACE_BEGIN(clixx)

class [[nodiscard]] Option
{
    std::string m_name;
    std::vector<Value> m_values;

public:
    constexpr Option(const char* name, std::vector<Value> args)
        : m_name(Valid(name)? name : "INVALID_OPTION"), m_values(args)
    {}

    static constexpr bool Valid(const std::string& str) {
        bool is = false;

        if (str.empty()) return false;

        if (str.starts_with("-") && not_at('-', str, 1) && str.size() == 2) {
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

    std::string fmt() const {
        std::string formatted = std::format("{}\n", name());

        for (auto& val : m_values) {
            formatted.append(std::format("  {}\n", val.str()));
        }

        return formatted;
    }
};


NAMESPACE_END()
