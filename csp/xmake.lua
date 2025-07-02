add_rules("mode.debug", "mode.release")
if is_plat("windows") then
    add_defines("/std:c++11")
else
    set_languages("c++11")
end

target("csp")
    set_kind("binary")
    add_includedirs(".")
    add_files("*.cxx")
    add_deps("sxtwl")
    add_deps("qimen")
    add_deps("zhcn")
    add_deps("base")
    add_deps("calendar")
