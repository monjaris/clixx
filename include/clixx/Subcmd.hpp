#pragma once
#include "core/defines.hpp"
#include "Option.hpp"
NAMESPACE_BEGIN(clixx)


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



NAMESPACE_END()
