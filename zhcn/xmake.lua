add_rules("mode.debug", "mode.release")
if is_plat("windows") then
    add_defines("/std:c++11")
else
    set_languages("c++11")
end
add_includedirs(os.scriptdir(), {public = true})
target("zhcn")
    set_kind("static")
    add_files("*.cxx")
