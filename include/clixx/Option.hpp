#pragma once
#include <vector>
#include <optional>
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

    struct {
        std::string name;
        std::vector<Value> values;
    } m;

    static bool Valid(const std::string& str);
    Option& pushValue(Value arg);

public:
    Option(const char* name, std::vector<Value> values, bool force_name = false);

    // Not intended for literal comparison, use `.is()` method instead
    const std::string& name() const;

    // Whether this option's name is equal to `str`
    bool is(const char* str) const;

    // Whether this option's name is equal to `short_opt` or `long_opt`
    bool is(const char* short_opt, const char* long_opt) const;

    // Whether this option's name is equal to any of the span's elements
    bool is(std::vector<const char*> any_of_str) const;

    // Variadic-Arguments version of `.is()` method
    template<class... T> requires AllIdConcept<const char*, T...>
    bool isV(T&&... any_of_str) const {
        return ((m.name == static_cast<T&&>(any_of_str)), ...);
    }

#if defined(CLIXX_OPTION_OPERATORS)
    // operator version of `.get()` method
    std::optional<Value> operator[] (usize at) {
        return this->get(at);
    }

    // operator version of `.is()` method
    bool operator() (const char* cstr) const {
        return this->is(cstr);
    }
    bool operator() (const char* short_opt, const char* long_opt) const {
        return this->is(short_opt, long_opt);
    }
    bool operator() (std::vector<const char*> cstrings) const {
        return this->is(cstrings);
    }
    template<class... T> requires AllIdConcept<const char*, T...>
    bool operator() (T&&... cstrings) const {
        return this->is(static_cast<T&&>(cstrings)...);
    }
#endif

    // get the value of this option at `i`
    template<bool bounds_check = true>
    std::optional<Value> get(usize at) const {
        if constexpr (bounds_check) {
            if (at >= count()) {
                return std::nullopt;
            }
        }

        return m.values[at];
    }

    // get this option's values in a vector
    auto values() const -> const std::vector<Value>&;
    // get string value of every `Value` in this option 
    auto valuesAsStrings() const -> std::vector<std::string>;

    // get the represantion(pretty-formatted) string value of option
    std::string repr() const;

    // get the option's value count
    usize count() const;

    // parse all options after `idx` at `argv` with the max length of `argc`
    static Option Parse(int argc, char** argv, int& idx);
};


NAMESPACE_END()
