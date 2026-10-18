#pragma once
#include "clixx/core/defines.hpp"
#include <string>
CLIIXX_NAMESPACE_BEGIN()

class [[nodiscard]] Value
{
    std::string m_default;
    std::string m_value;

public:
    Value(const char* value);

    // get string value of the value
    const std::string& val() const;

    // get the default string value(set at construction)
    const std::string& defaultVal() const;

    // generate string represantation of the value
    std::string repr(bool post_comma = false) const;
};

CLIXX_NAMESPACE_END()
