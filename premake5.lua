require "premake.project"
require "premake.command"
require "premake.links"
require "premake.outdir"


workspace "TankEngine"
	local engineName = "TankEngine"
	local editorName = "TankEditor"
	local playerName = "TankPlayer"

	local binDir = BinDir()
	local engineDir = binDir .. string.format("%s", engineName) .. "/%{cfg.longname}"
	local editorDir = binDir .. string.format("%s", editorName) .. "/%{cfg.longname}"
	local playerDir = binDir .. string.format("%s", playerName) .. "/%{cfg.longname}"

    configurations { "Debug", "Release" }
	architecture "x86_64"

    filter { "configurations:Debug" }
	symbols "On"
    filter { "configurations:Release" }
	optimize "On"


-- project "TankCSharpBindings"
-- 	kind "SharedLib"
-- 	PrjUseCSharp()
-- 	PrjObjAndTargetDir()

-- 	includedirs {
-- 		"%{prj.name}/Source"
-- 	}

-- 	files {
-- 		"%{prj.name}/Source/*.cs"
-- 	}

project "TankEngine"
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
	LinkFreetypePostCopy("lib", engineDir)

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/application.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

	-- Copy bin and libs into all applicable outdirs
	PostCopyDir(engineDir, editorDir)
	PostCopyDir(engineDir, playerDir)

project "TankEditor"
	kind "ConsoleApp"
	PrjUseCpp()
	PrjObjAndTargetDir()

	defines {
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"GLAD_GLAPI_EXPORT",
		"TANK_IMGUI"
	}

	includedirs {
		"include",
		"include/glm",
		"include/imgui",
		"%{prj.name}/include",
		"%{prj.name}/src",
		engineName .. "/src",
		engineName, -- for pch
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
		engineName
--		"mono-2.0-sgen"
	}
	LinkGLFW(_ACTION, editorDir)
	LinkAssimpPostCopy("lib", editorDir)

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/editor.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

project "TankPlayer"
	kind "ConsoleApp"
	PrjUseCpp()
	PrjObjAndTargetDir()

	defines {
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"GLAD_GLAPI_EXPORT"
	}
	
	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"include",
		"include/glm",
		"%{prj.name}/include",
		"%{prj.name}/src",
		engineName .. "/src",
		engineName, -- for pch
	}

	-- Library dirs
	libdirs {
		"lib",
	}
	LibDirWithPostCopy("%{prj.name}/lib", playerDir)

	-- Linked libraries
	LinkAssimpPostCopy("lib", playerDir)
	links {
		engineName
	}

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/player.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }


-- require "TankCSharpHost.premake5"
-- group "TankCSharpHost"
-- 	project "NativeHost"
-- 		NativeHostProject()
-- 	project "DotNetLib"
-- 		DotNetLibProject()
-- group ""