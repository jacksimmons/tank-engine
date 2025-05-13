workspace "TankEngine"
    local appDir = "Builds/bin/Application/%{cfg.longname}"

    configurations { "Debug", "Release" }
    architecture "x86_64"

    filter { "configurations:Debug" }
	symbols "On"
    filter { "configurations:Release" }
	optimize "On"

    project "Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "c++20"
	targetdir "Builds/bin/%{prj.name}/%{cfg.longname}"
	objdir "Builds/obj/%{prj.name}/%{cfg.longname}"

        defines {
                "TANK_DLL",
                "GLM_ENABLE_EXPERIMENTAL",
                "FMT_UNICODE=0",
                "ROOT_DIRECTORY=\"%{wks.location}\""
        }

        includedirs {
                "include",
                "include/glm",
                "%{prj.name}/include",
                "%{prj.name}/src",
                "%{prj.name}" -- for pch
        }
        pchheader "%{prj.name}/tepch.hpp"
        pchsource "%{prj.name}/application.cpp"
        files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}
        libdirs {
		"lib",
                "%{prj.name}/lib"
	}
        links {
                "assimp5"
	}

	postbuildcommands {
		"mkdir -p " .. appDir,
		"{COPYFILE} %[%{cfg.buildtarget.relpath}] %[" .. appDir .. "/%{cfg.buildtarget.name}]"
	}

    project "Application"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++20"
	targetdir "Builds/bin/%{prj.name}/%{cfg.longname}"
	objdir "Builds/obj/%{prj.name}/%{cfg.longname}"

	defines {
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"ROOT_DIRECTORY=\"%{wks.location}\"",
		"APP_DIRECTORY=\"%{prj.location}\""
	}

        pchheader "Engine/tepch.hpp"

	includedirs {
		"include",
		"include/glm",
		"%{prj.name}/include",
		"%{prj.name}/src",
		"Engine/src",
		"Engine", -- for pch
                "%{prj.name}/include/imgui",
                "%{prj.name}/include/imgui/backends",
                "vendor/mono/include"
	}

        files {
                "%{prj.name}/src/**.hpp",
                "%{prj.name}/src/**.cpp",
		"%{prj.name}/include/imgui/imgui*.cpp",
		"%{prj.name}/include/imgui/backends/imgui_impl_glfw.cpp",
		"%{prj.name}/include/imgui/backends/imgui_impl_opengl3.cpp",
		"%{prj.name}/include/glad/glad.cpp"
        }

	local glfwDir = ""
	local glfwLib = ""
	filter { "system:not windows" }
		glfwDir = "%{prj.name}/lib/glfw/lib-gcc"
		glfwName = "libglfw3.so"

	libdirs {
		"lib",
		"%{prj.name}/lib",
--		"vendor/mono/lib/%{cfg.buildtarget.name}",
		glfwDir
	}

	links {
		"Engine",
		"glfw3",
		"assimp5",
--		"mono-2.0-sgen"
	}

	filter { "system:windows" }
		links { "OpenGL32", "glfw3dll" }
	filter { "system:not windows" }
		links { "GL" }
        
        prebuildcommands {
	        "{COPYFILE} %[" .. glfwDir .. "/" .. glfwName .. "] %[" .. appDir .. "/" .. glfwName .. "]"
        }
