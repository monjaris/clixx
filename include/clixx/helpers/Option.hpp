#pragma once
#include <vector>
#include "clixx/core/utils.hpp"
#include "Value.hpp"
NAMESPACE_BEGIN(clixx)


// For library to remain small, i added feature macros so devs can opt-in:
//
// CLIXX_OPTION_OPERATORS
    // you get operator overloads for Option class such as opt("-o", "--opt") and opt[1]


#define CLIXX_OPTION_OPERATORS


class Option
{
    friend class CLI;
    std::string m_name;
    std::vector<Value> m_values;


    static constexpr bool Valid(std::string str) {
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

    Option& pushValue(Value arg) {
        m_values.push_back(arg);
        return *this;
    }


public:
    constexpr Option(const char* name, std::vector<Value> values)
        : m_values(values)
    {
        if (!Valid(name)) {
            m_name = {};
        } else {
            m_name = name;
        }
    }

    // not intented for literal comparison, use `.is()` method instead
    const std::string& name() const {
        return m_name;
    }

    // wheter option's name is equal to `str`
    constexpr bool is(const char* str) const {
        return m_name == str;
    }

    // (overload) // wheter option's name is equal to `short_opt` or `long_opt`
    // argument names are only semantic, you can pass long one to `short_opt` too!
    constexpr bool is(const char* short_opt, const char* long_opt) const {
        return m_name == short_opt || m_name == long_opt;
    }

    // (overload) // wheter option's name is equal to any of the span's elements
    constexpr bool is(std::span<const char*> any_of_str) const {
        for (usize i=0;  i < any_of_str.size();  ++i) {
            if (m_name == any_of_str[i]) return true;
        }
        return false;
    }

    // Variadic-Arguments version of `.is()` method
    template<class... T> requires AllIdConcept<const char*, T...>
    bool isV(T&&... any_of_str) const {
        return ((m_name == static_cast<T&&>(any_of_str)), ...);
    }



#if defined(CLIXX_OPTION_OPERATORS)
    // operator version of `.get()` method
    constexpr std::optional<Value> operator[] (usize at) {
        return this->get(at);
    }

    // operator version of `.is()` method
    constexpr bool operator() (const char* cstr) const {
        return this->is(cstr);
    }
    // operator version of `.is()` method
    constexpr bool operator() (const char* short_opt, const char* long_opt) const {
        return this->is(short_opt, long_opt);
    }
    // operator version of `.is()` method
    constexpr bool operator() (std::span<const char*> cstrings) const {
        return this->is(cstrings);
    }
    // operator version of `.is()` method
    template<class... T> requires AllIdConcept<const char*, T...>
    constexpr bool operator() (T&&... cstrings) const {
        return this->is(static_cast<T&&>(cstrings)...);
    }


#endif


    // get the value of the option at `i`
    // if template argument `bounds_check` is `false` then this method wont ever return `nullopt`!
    template<bool bounds_check = true>
    constexpr std::optional<Value> get(usize at) const {
        if constexpr (bounds_check) {
            if (at >= count()) {
                return std::nullopt;
            }
        }

        return m_values[at];
    }


    // get the option's values in a vector
    // note: not intented for value access, use `.at()` method instead
    constexpr auto values() const -> const std::vector<Value>& {
        return m_values;
    }


    // get the option's value count
    constexpr usize count() const {
        return m_values.size();
    }



    // parse all options after `idx` at `argv` with the max length of `argc`
    static constexpr Option parse(int argc, char** argv, int& idx) {
        Option option = {argv[idx], {}};
        ++idx;

        for (;  idx < argc;  ++idx) {
            const auto& maybe_arg = argv[idx];

            // found next option, yield
            if (Option::Valid(maybe_arg)) {
                --idx; break;
            }

            option.pushValue(Value(maybe_arg));
        }

        return option;
    }
};


NAMESPACE_END()
