#include <format>
#include "clixx/Value.hpp"
NAMESPACE_BEGIN(clixx)

Value::Value(const char* value)
    : m_default(value), m_value(value)
{}


const std::string& Value::val() const {
    return m_value;
}


const std::string& Value::defaultVal() const {
    return m_value;
}


std::string Value::repr(bool post_comma) const {
    return m_value + (post_comma? ", " : "");
}


NAMESPACE_END()
