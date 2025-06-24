require "premake.project"
require "premake.command"
require "premake.links"


local function includeOpenGL()

end


workspace "TankEngine"
	local engineDir = "Builds/bin/Engine/%{cfg.longname}"
    local editorDir = "Builds/bin/Editor/%{cfg.longname}"
    local playerDir = "Builds/bin/Player/%{cfg.longname}"

    configurations { "Debug", "Release" }
    architecture "x86_64"

    filter { "configurations:Debug" }
	symbols "On"
    filter { "configurations:Release" }
	optimize "On"

project "Engine"
	kind "SharedLib"
	PrjUseCpp()
	PrjObjAndTargetDir()

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

	-- Library dirs
	libdirs {
		"lib"
	}
	LibDirWithPostCopy("%{prj.name}/lib", engineDir)
	LibDirGLFWPostCopy(_ACTION, engineDir)
	
	-- Linked libraries
	LinkGLFW()
	LinkOpenGL()
	LinkAssimpPostCopy("lib", engineDir)

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/application.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

	-- Copy bin and libs into all applicable outdirs
	PostCopyDir(engineDir, editorDir)
	PostCopyDir(engineDir, playerDir)

project "Editor"
	kind "ConsoleApp"
	PrjUseCpp()
	PrjObjAndTargetDir()

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

	-- Library dirs
	libdirs {
		"lib",
	}
	LibDirWithPostCopy("%{prj.name}/lib", editorDir)
	LibDirGLFWPostCopy(_ACTION, editorDir)

	-- Linked libraries
	links {
		"Engine",
--		"mono-2.0-sgen"
	}
	LinkGLFW(_ACTION, editorDir)
	LinkAssimpPostCopy("lib", editorDir)

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/editor.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

project "Player"
	kind "ConsoleApp"
	PrjUseCpp()
	PrjObjAndTargetDir()

	defines {
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"GLAD_GLAPI_EXPORT"
	}
	
	files {
		"include/imgui/imgui*.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"include",
		"include/glm",
		"include/imgui",
		"%{prj.name}/include",
		"%{prj.name}/src",
		"Engine/src",
		"Engine", -- for pch
	}

	-- Library dirs
	libdirs {
		"lib",
	}
	LibDirWithPostCopy("%{prj.name}/lib", playerDir)

	-- Linked libraries
	LinkAssimpPostCopy("lib", playerDir)
	links {
		"Engine"
	}