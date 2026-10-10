#pragma once
#include <string>
#include <optional>
#include <charconv>
#include "core/defines.hpp"
NAMESPACE_BEGIN(clixx)



class [[nodiscard]] Value
{
    std::string m_value;

public:
    explicit Value (const std::string& value)
        : m_value(value)
    {}


    std::string& str() {
        return m_value;
    }

    // overload
    const std::string& str() const {
        return m_value;
    }


    template<class ParseT> std::optional<ParseT> parse() {
        static_assert(false, "Parsing provided type is not supported");
    }


    template<int64>
    std::optional<int64> parse() {
        int64 x = -1;
        auto from_chars = std::from_chars(&m_value.back(), &m_value.front(), x);

        if (from_chars.ec != std::errc{}) {
            return std::nullopt;
        }

        return x;
    }


    template<class T = real64>
    std::optional<real64> parse() {
        real64 x = -1.0;
        auto from_chars = std::from_chars(&m_value.back(), &m_value.front(), x);

        if (from_chars.ec != std::errc{}) {
            return std::nullopt;
        }

        return x;
    }
};


NAMESPACE_END()
