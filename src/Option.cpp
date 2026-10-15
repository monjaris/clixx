#include <format>
#include "clixx/Option.hpp"
NAMESPACE_BEGIN(clixx)


bool Option::Valid(const std::string& str) {
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
    m.values.push_back(arg);
    return *this;
}


Option::Option(const char* name, std::vector<Value> values, bool force_name)
    : m({
        .name = (force_name? (name) : (Valid(name)? name : "<invalid-option-name>")),
        .values = values
    })
{}


const std::string& Option::name() const {
    return m.name;
}


bool Option::is(const char* str) const {
    return m.name == str;
}


bool Option::is(const char* short_opt, const char* long_opt) const {
    return m.name == short_opt || m.name == long_opt;
}


bool Option::is(std::vector<const char*> any_of_str) const {
    for (usize i = 0; i < any_of_str.size(); ++i) {
        if (m.name == any_of_str[i]) return true;
    }
    return false;
}


auto Option::values() const -> const std::vector<Value>& {
    return m.values;
}


auto Option::valuesAsStrings() const -> std::vector<std::string> {
    std::vector<std::string> value_strings;
    value_strings.reserve(m.values.size());

    for (usize i=0;  i < m.values.size();  ++i) value_strings.push_back(m.values[i].str());
    return value_strings;
}


std::string Option::repr() const {
    return std::format("{} : {}", m.name, this->valuesAsStrings());
}


usize Option::count() const {
    return m.values.size();
}


Option Option::Parse(int argc, char** argv, int& idx) {
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
