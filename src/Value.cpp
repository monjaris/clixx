#include <format>
#include "clixx/Value.hpp"
NAMESPACE_BEGIN(clixx)

Value::Value(std::string value)
    : m_value(std::move(value))
{}


std::string& Value::str() {
    return m_value;
}


const std::string& Value::str() const {
    return m_value;
}


bool Value::Valid(const std::string& str) {
    return !str.empty();
}


std::string Value::repr(bool post_comma) const {
    return std::format("{}{} ", m_value, post_comma ? "," : "");
}


NAMESPACE_END()
