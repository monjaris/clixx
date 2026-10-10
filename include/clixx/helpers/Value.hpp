#pragma once
#include "clixx/core/defines.hpp"
#include <string>
#include <format>
NAMESPACE_BEGIN(clixx)

class [[nodiscard]] Value
{
    std::string m_value;
public:
    explicit Value(std::string value)
        : m_value(value)
    {}

    auto& str() { return m_value; }
    const auto& str() const { return m_value; }


    static bool Valid(const std::string& str) {
        return !str.empty();
    }


    std::string repr(bool post_comma = false) const {
        return std::format("{}{} ", m_value, post_comma? "," : "");
    }
};


NAMESPACE_END()
