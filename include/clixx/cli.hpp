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


template<class ContainerT> [[nodiscard]]
bool not_at(FetchValueT<ContainerT> value, ContainerT cont, usize idx) {
    if (idx > cont.size()) return false;

    return cont[idx] != value;
}



class [[nodiscard]] Option
{
    std::string m_name;
    std::vector<std::string> m_args;

public:
    constexpr Option(const char* name, std::vector<std::string> args)
        : m_name(Valid(name)? name : "INVALID_OPTION"), m_args(args)
    {}

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

    std::string fmt() const {
        std::string formatted = std::format("{} {}", m_name, m_args);
        return formatted;
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



class Subcmd
{
    std::string m_name;
    std::vector<Option> m_options;

public:
    constexpr Subcmd(const char* name, std::vector<Option> options)
        : m_name(Valid(name)? name : "INVALID_SUBCOMMAND"), m_options(options)
    {}

    static constexpr bool Valid(const std::string& str) {
        bool is = true;

        if (str.empty()) return false;

        if (Option::Valid(str)) is = false;

        return is;
    }

    const std::string& name() const {
        return m_name;
    }

    constexpr usize count() const {
        return m_options.size();
    }

    std::optional<Option> get(usize i) const {
        if (i > count()-1) {
            return std::nullopt;
        } else {
            return m_options[i];
        }
    }

    std::string fmt() const {
        static std::string formatted = std::format("{}\n", name());

        for (auto& opt : m_options) {
            formatted.append(std::format("  {}\n", opt.fmt()));
        }

        return formatted;
    }
};




class CLI
{
    struct M {
        char** argv = nullptr;
        int argc = -1;
        std::vector<std::string> data;
        std::vector<Option> options;
        std::vector<Subcmd> subcommands;
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
        m.argv = argv;

        m.data = {argv, argv + argc};
        m.options.reserve(128);
    }

    [[noreturn]] void exit(const char* msg, int exit_code = 0) {
        std::println("{}", msg);
        ::exit(exit_code);
    }


    void printOptions() {
        for (const auto& option : m.options) {
            std::println("{}", option.fmt());
        }
    }
    void printSubcmds() {
        for (const auto& subcmd : m.subcommands) {
            std::println("{}", subcmd.fmt());
        }
    }
    void printOrphans() {
        for (const auto& orp : m.orphans) {
            std::print("{}; ", orp);
        }
        std::println();
    }


    const std::vector<Option>& options() {
        return m.options;
    }

    const std::vector<Subcmd>& subcommands() {
        return m.subcommands;
    }


    Option parseOption(char** argv, int& idx) {
        Option option = {argv[idx], {}};
        ++idx;

        for (;  idx < m.argc;  ++idx) {
            const auto& maybe_arg = m.data[idx];

            // found next option, yield
            if (Option::Valid(maybe_arg)) {
                --idx; break;
            }

            option.pushArg(maybe_arg);
        }

        return option;
    }

    Subcmd parseSubcmd(char** argv, int& idx) {
        Subcmd subcmd = {argv[idx], {}};

        for (;  idx < m.argc;  ++idx) {
            const auto& maybe_arg = m.data[idx];

            // found next subcommand, yield
            if (Subcmd::Valid(maybe_arg)) break;
        }

        return subcmd;
    }


    [[nodiscard]] bool parse() {
        for (int i=1;  i < m.argc;  ++i) {
            auto& str = m.data[i];

            bool subcommand_found = false;

            if (Subcmd::Valid(str)) {
                std::println("subcmd found!");
                subcommand_found = true;
                
            }
            if (Option::Valid(str) && !subcommand_found) {
                m.options.push_back(parseOption(m.argv, i));
            }
            else {
                m.orphans.push_back(str);
            }
        }

        return true;
    }
};



NAMESPACE_END()
