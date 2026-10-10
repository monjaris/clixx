--- RULES/POLICIES
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")
add_rules("plugin.compile_commands.autoupdate")


--- SCRIPT-BEGIN
    option("pedantic", {default = true, description = "Toggle-on for pedantic compile flags"})

    if is_mode("debug") then
        optimize = "-O0"
        lto = "-fno-lto"
        emit_stack = "-fno-omit-frame-pointer"
        if has_config("pedantic") then
            add_cxxflags("-Wshadow", "-Wundef", "-Wcast-align", "-Wdouble-promotion")
        end

    elseif is_mode("release") then
        optimize = "-O3"
        lto = "-flto"
        emit_stack = ""
    end
--- SCRIPT-END



--- CONFIG
cxx_flags = {
    "-Wall", "-Wextra",
    "-fno-exceptions", "-fno-rtti",
    optimize, lto, emit_stack,
}
ld_flags = {
    lto
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


--- TEST-TARGETS
target("main") add_files("test/main.cpp") add_deps("clixx")
