#pragma once
#include "clixx/core/defines.hpp"
#include <string>
NAMESPACE_BEGIN(clixx)

class [[nodiscard]] Value
{
    std::string m_value;

public:
    explicit Value(std::string value);

    std::string& str();
    const std::string& str() const;

    static bool Valid(const std::string& str);

    std::string repr(bool post_comma = false) const;
};

NAMESPACE_END()
