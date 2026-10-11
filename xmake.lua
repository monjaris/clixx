--- RULES/POLICIES
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")
add_rules("plugin.compile_commands.autoupdate")


--- SCRIPT-BEGIN
    ---@diagnostic disable-next-line -- because lsp is not always correct..
    option("pedantic", {default = true, description = "Toggle-on for pedantic compile flags"})

    if is_mode("debug") then
        add_cxxflags("-O0", "-fno-lto", "-fno-omit-frame-pointer")
        if has_config("pedantic") then
            add_cxxflags("-Wshadow", "-Wundef", "-Wcast-align", "-Wdouble-promotion")
        end

    elseif is_mode("release") then
        add_cxxflags("-O3", "-flto")
    end
--- SCRIPT-END



--- CONFIG
cxx_flags = {
    "-Wall", "-Wextra",
    "-fno-exceptions", "-fno-rtti",
}
ld_flags = {
    ""
}
set_languages("c++23")
add_includedirs("include/")
add_cxxflags(table.unpack(cxx_flags))
add_ldflags(table.unpack(ld_flags))


after_build(function (target)
    os.cp(target:targetfile(), "$(projectdir)/")
end)


--- TARGETS
target("clixx")
    set_kind("$(kind)")
    add_files("src/*.cpp")


--- TESTS
target("main") add_files("test/main.cpp") add_deps("clixx")
target("option_methods") add_files("test/option_methods.cpp") add_deps("clixx")
