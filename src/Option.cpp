#include "clixx/Option.hpp"
NAMESPACE_BEGIN(clixx)


bool Option::Valid(std::string str) {
    bool is = false;

    if (str.empty()) return false;
    char c = '-';

    if (str.starts_with("-") && util::not_at(c, str, 1) && str.size() == 2) {
        is = true;
    }

    if (str.starts_with("--") && util::not_at('-', str, 2) && str.size() > 2) {
        is = true;
    }

    return is;
}


Option& Option::pushValue(Value arg) {
    m_values.push_back(arg);
    return *this;
}


Option::Option(const char* name, std::vector<Value> values)
    : m_values(values)
{
    if (!Valid(name)) {
        m_name = {};
    } else {
        m_name = name;
    }
}


const std::string& Option::name() const {
    return m_name;
}


bool Option::is(const char* str) const {
    return m_name == str;
}


bool Option::is(const char* short_opt, const char* long_opt) const {
    return m_name == short_opt || m_name == long_opt;
}


bool Option::is(std::vector<const char*> any_of_str) const {
    for (usize i = 0; i < any_of_str.size(); ++i) {
        if (m_name == any_of_str[i]) return true;
    }
    return false;
}


auto Option::values() const -> const std::vector<Value>& {
    return m_values;
}


usize Option::count() const {
    return m_values.size();
}


Option Option::parse(int argc, char** argv, int& idx) {
    Option option = {argv[idx], {}};
    ++idx;

    for (; idx < argc; ++idx) {
        const auto& maybe_arg = argv[idx];

        // Found next option, yield
        if (Option::Valid(maybe_arg)) {
            --idx; 
            break;
        }

        option.pushValue(Value(maybe_arg));
    }

    return option;
}


NAMESPACE_END()
