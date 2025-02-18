add_rules("mode.debug", "mode.release")
set_languages("c++11")
if is_plat("windows") then
	add_cxxflags("/source-charset:utf-8")
end
if is_plat("mingw") then
	add_cxxflags("-Wno-unused-variable -finput-charset=utf-8 -fexec-charset=gbk")
end
includes("calendar", "sxtwl", "qimen", "zhcn")
target("csp")
    set_kind("binary")
    add_includedirs(path.join(os.scriptdir(), "src"))
    add_files("src/*.cxx")
    add_deps("calendar")
    add_deps("sxtwl")
    add_deps("qm")
    add_deps("zhcn")
