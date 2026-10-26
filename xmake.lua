--- RULES/POLICIES
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")


--- SCRIPT-BEGIN
    ---@diagnostic disable-next-line -- because lsp is not always correct..
    option("pedantic", {default = true, description = "Toggle-on for pedantic compile flags"})

    if is_mode("debug") then
        if has_config("pedantic") then
            add_cxxflags("-Wshadow", "-Wundef", "-Wcast-align", "-Wdouble-promotion")
        end
    end
--- SCRIPT-END



--- CONFIG
add_includedirs("include/", {public = true})
add_cxxflags(
    "-std=c++23",
    "-Wall", "-Wextra",
    "-fno-exceptions", "-fno-rtti"
)


after_build(function (target)
    os.cp(target:targetfile(), "$(projectdir)/")
end)


--- TARGETS
target("clixx")
    set_kind("$(kind)")
    add_files("src/*.cpp")
    add_headerfiles("include/(clixx/**.hpp)")

    if is_kind("static") then
        after_build(function (target)
            local targetfile = target:targetfile()
            local so_file = targetfile:match("^(.*)%.[^.]+$")..".so"
            os.rm(so_file)
        end)
    end


--- TESTS
target("main") add_files("test/main.cpp") add_deps("clixx")
target("option_methods") add_files("test/option_methods.cpp") add_deps("clixx")
