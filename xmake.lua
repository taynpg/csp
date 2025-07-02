add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

if is_plat("windows") then
	add_cxxflags("/utf-8")
    add_defines("/std:c++11")
else
    set_languages("c++11")
end

if is_mode("debug") then
    set_suffixname("d")
end

includes("sxtwl", "calendar", "base", "qimen", "zhcn", "csp")
