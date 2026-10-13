#include <format>
#include "clixx/Value.hpp"
NAMESPACE_BEGIN(clixx)

Value::Value(const char* value)
    : m_value(std::move(value))
{}


std::string& Value::str() {
    return m_value;
}


const std::string& Value::str() const {
    return m_value;
}


bool Value::Valid(const char* str) {
    return str != nullptr;
}


std::string Value::repr(bool post_comma) const {
    return m_value + (post_comma? ", " : "");
}


NAMESPACE_END()
