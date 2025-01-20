-- set project name
set_project("tetris")

-- set project version
set_version("1.0.0")

-- set language standard
set_languages("c++23")

-- set build and bin directories
set_objectdir("build")
set_targetdir("bin")

add_requires("spdlog", "sfml")

set_allowedmodes("debug", "release")

-- set default mode
set_defaultmode("debug")

-- add target
target("tetris")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("spdlog", "sfml")
    add_cxxflags("-Wall", "-Wextra", "-Werror", "-pedantic", "-g")
    add_ldflags("-ObjC")
    -- set debug mode
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("DEBUG")
    end
-- add include directories
add_includedirs("include")