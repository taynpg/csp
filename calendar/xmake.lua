add_rules("mode.debug", "mode.release")
if is_plat("windows") then
    add_defines("/std:c++11")
else
    set_languages("c++11")
end
add_includedirs(path.join(os.scriptdir(), "include"), {public = true})
target("calendar")
    set_kind("static")
    add_files("src/*.cxx")
    add_deps("sxtwl")
