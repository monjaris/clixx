#pragma once
#include "core/defines.hpp"
#include <expected>
#include <string>
#include <vector>
#include <print>
NAMESPACE_BEGIN(clixx)



// CLIXX
// basic definitions:
// options -> `./prog --option x` || `./prog -o x y z` || `./prog --opt`


template<class T>
constexpr auto getValue() {
    if constexpr (requires { typename T::value_type; }) {
        return std::type_identity<typename T::value_type>();
    }
    if constexpr (requires { typename T::value_type; }) {
        return std::type_identity<typename T::value_type>();
    }
    else {
        static_assert(sizeof(T) == 0, "Could not resolve value type");
    }
}


template<class T>
using FetchValueT = decltype(getValue<T>())::type;


template<class ContainerT>
bool not_at(FetchValueT<ContainerT> value, ContainerT cont, usize idx) {
    if (idx > cont.size()) return false;

    return cont[idx] != value;
}


class Option
{
    std::string m_name;
    std::vector<std::string> m_args;

public:
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wuninitialized"
    constexpr Option(const char* name, std::vector<std::string> args)
        : m_args(args)
    {
        if (!Valid(name)) {
            m_name = {};
        } else {
            m_name = name;
        }
    }
#pragma GCC diagnostic pop

    static constexpr bool Valid(const std::string& str) {
        bool is = false;

        if (str.empty()) return false;

        if (str.starts_with("-") && not_at('-', str, 1) && str.size() == 2) {
            is = true;
        }

        if (str.starts_with("--") && not_at('-', str, 2) && str.size() > 2) {
            is = true;
        }

        return is;
    }

    const std::string& name() const {
        return m_name;
    }

    constexpr usize count() const {
        return m_args.size();
    }

    constexpr bool isFlag() const {
        return count() == 0;
    }

    std::optional<std::string> get(usize i) const {
        if (i > count()-1) {
            return std::nullopt;
        } else {
            return m_args[i];
        }
    }

    auto args() const -> const std::vector<std::string>& {
        return m_args;
    }

    Option& pushArg(std::string arg) {
        m_args.push_back(arg);
        return *this;
    }


    template<class ParseT> std::optional<ParseT> parse() {
        static_assert(false, "Parsing provided type is not supported");
    }

    template<int64>
    std::optional<int64> parse() {
        int64 x = -1;
        auto from_chars = std::from_chars(&m_name.back(), &m_name.front(), x);

        if (from_chars.ec != std::errc{}) {
            return std::nullopt;
        }

        return x;
    }

    template<class T = real64>
    std::optional<real64> parse() {
        real64 x = -1.0;
        auto from_chars = std::from_chars(&m_name.back(), &m_name.front(), x);

        if (from_chars.ec != std::errc{}) {
            return std::nullopt;
        }

        return x;
    }
};



class CLI
{
    struct M {
        int argc = -1;
        std::vector<std::string> data;
        std::vector<Option> options;
        std::vector<std::string> orphans;
    } m;

public:

    class ParseResult {
        bool m_result;
    public:
        ParseResult(bool success): m_result(success) {}

        bool operator! () {
            return m_result;
        }
    };


    CLI (int argc, char** argv)
    {
        m.argc = argc;
        m.data = {argv, argv + argc};

        m.options.reserve(128);
    }

    [[noreturn]] void exit(const char* msg, int exit_code = 0) {
        std::println("{}", msg);
        ::exit(exit_code);
    }


    void printOptions() {
        for (const auto& option : m.options) {
            std::println("{}: {}", option.name(), option.args());
        }
    }

    const std::vector<Option>& options() {
        return m.options;
    }


    [[nodiscard]] bool parse() {
        for (int i=1;  i < m.argc;  ++i) {
            std::string& str = m.data[i];

            if (Option::Valid(str)) {
                Option option = {str.c_str(), {}};

                for (int j=i + 1;  j < m.argc;  ++j) {
                    const auto& maybe_arg = m.data[j];

                    if (Option::Valid(maybe_arg)) break;

                    option.pushArg(maybe_arg);
                }

                m.options.push_back(option);
            }
            else {
                m.orphans.push_back(str);
            }
        }

        return true;
    }
};



NAMESPACE_END()
