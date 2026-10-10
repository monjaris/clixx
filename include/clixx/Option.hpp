#pragma once
#include <string>
#include <vector>
#include <optional>
#include <charconv>
#include <format>
#include "core/defines.hpp"
#include "core/utils.hpp"
NAMESPACE_BEGIN(clixx)

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


NAMESPACE_END()
