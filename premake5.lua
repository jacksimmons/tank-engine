local function includeGlfw()
	local glfwDir = "lib/glfw/lib-" .. _ACTION
	libdirs { glfwDir }
	links { "glfw3" }
	return glfwDir;
end


local function includeAssimp()
	filter { "system:windows" }
		links { "assimp-vc143-mt" }
	filter { "system:not windows" }
		links { "assimp5" }
end


local function includeOpenGL()
	filter { "system:windows" }
		links { "OpenGL32" }
	filter { "system:not windows" }
		links { "GL" }
end


workspace "TankEngine"
    local editorDir = "Builds/bin/Editor/%{cfg.longname}"

    configurations { "Debug", "Release" }
    architecture "x86_64"

    filter { "configurations:Debug" }
	symbols "On"
    filter { "configurations:Release" }
	optimize "On"

project "Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "c++23"
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
		"include/imgui",
		"include/imgui/backends",
		"%{prj.name}/include",
		"%{prj.name}/src",
		"%{prj.name}" -- for pch
	}

	files {
		"include/imgui/imgui*.cpp",
		"include/imgui/backends/imgui_impl_glfw.cpp",
		"include/imgui/backends/imgui_impl_opengl3.cpp",
		"include/glad/glad.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	-- Libraries
	libdirs {
		"lib",
		"%{prj.name}/lib",
	}
	local glfwDir = includeGlfw()
	includeAssimp()
	includeOpenGL()

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/application.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

	-- Copy DLL into Editor outdir
	postbuildcommands {
		"{MKDIR} " .. editorDir,
		"{COPYDIR} %[" .. glfwDir .. "] %[%{cfg.buildtarget.directory}]",
		"{COPYDIR} %[%{cfg.buildtarget.directory}] %[" .. editorDir .. "]"
	}

project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++23"
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
		"include/imgui",
		"%{prj.name}/include",
		"%{prj.name}/src",
		"Engine/src",
		"Engine", -- for pch
		"vendor/mono/include"
	}
	
	files {
		"include/imgui/imgui*.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	-- Libraries
	libdirs {
		"lib",
		"%{prj.name}/lib",
--		"vendor/mono/lib/%{cfg.buildtarget.name}",
	}
	includeGlfw()
	links {
		"Engine",
--		"mono-2.0-sgen"
	}

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/editor.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }
    
	-- Copy dependency DLLs into outdir
	postbuildcommands {
		"{COPYDIR} %[%{prj.name}/lib] %[" .. editorDir .. "]"
	}
