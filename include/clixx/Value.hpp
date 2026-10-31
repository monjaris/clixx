#pragma once
#include "core/defines.hpp"
#include <string>
CLIXX_NAMESPACE_BEGIN()


class [[nodiscard]] Value
{
    std::string m_default;
    std::string m_value;

public:
    Value(const char* value);

    // get string value of the value
    const std::string& str() const;

    // get the default string value(set at construction)
    const std::string& defaultValue() const;

    // generate string represantation of the value
    std::string repr(bool post_comma = false) const;
};

CLIXX_NAMESPACE_END()
