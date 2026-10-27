#include <print>
#include "include/test.hpp"
#include "clixx/clixx.hpp"

TEST_MAIN()
{
    using clixx::Value;
    using clixx::Option;

    clixx::Option option = {"--opt", { Value("2334"), Value("hello") }};

    if (option.is("--opt"))
    {
        std::println("[[ OPTION ]]");
        std::println("opt-name:      {}", option.name());
        std::println("repr-fmt:  {}", option.repr());
        std::println("arg-count: {}", option.count());
        std::println("first-arg: {}", option.get(0)->str());
    }
    else
    {
        TEST_QUIT(TEST_QUIT_ERR);
    }

    TEST_QUIT(TEST_QUIT_OK);
}
