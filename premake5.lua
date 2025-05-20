workspace "TankEngine"
    local appDir = "Builds\\bin\\Application\\%{cfg.longname}"

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
		"GLAD_GLAPI_EXPORT",
		"GLAD_GLAPI_EXPORT_BUILD"
	}

	includedirs {
		"include",
		"include/glm",
		"%{prj.name}/include",
		"%{prj.name}/src",
		"%{prj.name}" -- for pch
	}

	files {
		"include/glad/glad.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	libdirs {
		"lib",
		"%{prj.name}/lib"
	}

	-- Platform-specific links
	filter { "system:windows" }
		links { "assimp-vc143-mt", "OpenGL32" }
	filter { "system:not windows" }
		links { "assimp5", "GL" }

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/application.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

	-- Copy DLL into Application outdir
	postbuildcommands {
		"{MKDIR} " .. appDir,
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
		"GLAD_GLAPI_EXPORT"
	}

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
		"%{prj.name}/include/imgui/backends/imgui_impl_opengl3.cpp"
	}

	-- Library directories
	local glfwDir = "%{prj.name}/lib/glfw/lib-" .. _ACTION
	libdirs {
		"lib",
		"%{prj.name}/lib",
--		"vendor/mono/lib/%{cfg.buildtarget.name}",
		glfwDir
	}

	-- Library links
	links {
		"Engine",
		"glfw3",
--		"mono-2.0-sgen"
	}

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/editor.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }
    
	-- Copy dependency DLLs into outdir
	postbuildcommands {
		"{COPYDIR} %[" .. glfwDir .. "] %[" .. appDir .. "]",
		"{COPYDIR} %[lib] %[" .. appDir .. "]",
		"{COPYDIR} %[%{prj.name}/lib] %[" .. appDir .. "]"
	}
