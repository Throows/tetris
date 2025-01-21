-- set project name
set_project("tetris")

-- set project version
set_version("1.0.0")

-- set language standard
set_languages("c++23")

add_rules("mode.debug", "mode.releasedbg", "mode.release")

-- set build and bin directories
set_objectdir("./build/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")

add_requires("spdlog", "sfml")

set_allowedplats("windows", "macosx", "linux")
set_allowedmodes("debug", "release")
set_defaultmode("debug")

-- add target
target("tetris")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("spdlog", "sfml")
    add_cxxflags("-Wall", "-Wextra", "-Werror", "-pedantic")

    if is_plat("macosx") then
        add_ldflags("-ObjC")
        add_rules("xcode.application")
    end

    if is_plat("macosx") then
        add_defines("PLAT_DARWIN")
    elseif is_plat("linux") then
        add_defines("PLAT_LINUX")
    elseif is_plat("windows") then
        add_defines("PLAT_WINDOWS")
    end

    -- set debug mode
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("DEBUG")
        add_cxxflags("-g")
        if is_plat("windows") then
            add_ldflags("-SUBSYSTEM:CONSOLE")
            add_ldflags("-ENTRY:mainCRTStartup")
        end
    elseif is_mode("release") then
        set_symbols("none")
        set_optimize("fastest")
        add_defines("RELEASE")
        -- Windows
        if is_plat("windows") then
            add_ldflags("-ENTRY:WinMainCRTStartup")
            add_ldflags("-SUBSYSTEM:WINDOWS")
            add_defines("WIN_START")
        end 
    end

    -- add include directories
    add_includedirs("include")

    after_build(function (target)
        print("Target Built : %s", target:name())
        os.rm("bin/$(plat)_$(arch)_$(mode)/assets")
        os.cp("assets", "bin/$(plat)_$(arch)_$(mode)/assets")
        if is_plat("macosx") then
            os.cp("assets", "bin/$(plat)_$(arch)_$(mode)/tetris.app/Contents/MacOS/assets")
        end
    end)
target_end()