require "os"
require "premake.project"
require "premake.command"
require "premake.links"
require "premake.outdir"


workspace "TankEngine"
	local engineName = "TankEngine"
	local editorName = "TankEditor"
	local playerName = "TankPlayer"

	local binDir = BinDir()
	local engineDir = binDir .. engineName .. "/%{cfg.shortname}"
	local editorDir = binDir .. editorName .. "/%{cfg.shortname}"
	local playerDir = binDir .. playerName .. "/%{cfg.shortname}"

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

project "Lua"
	local luaDir = binDir .. "Lua/%{cfg.shortname}"
	kind "StaticLib"
	PrjUseC()
	PrjObjAndTargetDir()

	includedirs {
		engineName .. "/include/lua"
	}

	files {
		engineName .. "/include/lua/*.c"
	}

project "miniaudio"
	local miniaudioDir = binDir .. "miniaudio/%{cfg.shortname}"
	kind "StaticLib"
	PrjUseC()
	PrjObjAndTargetDir()

	includedirs {
		"vendor"
	}

	files {
		"vendor/miniaudio/miniaudio.c"
	}

project "TankEngine"
	kind "SharedLib"
	PrjUseCpp()
	PrjObjAndTargetDir()

	defines {
		"TANK_DLL",
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"GLAD_GLAPI_EXPORT",
		"GLAD_GLAPI_EXPORT_BUILD",
		"FFMPEG_DIR=\"vendor/ffmpeg\"",
		"STBI_NO_SIMD"
	}

	includedirs {
		"include",
		"include/glm",
		"include/imgui",
		"include/imgui/backends",
		"include/lua",
		"%{prj.name}/include",
		"%{prj.name}/include/lua",
		"%{prj.name}/src",
		"%{prj.name}", -- for pch
		"vendor"
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
	LibDirWithPostCopy(luaDir, engineDir)
	LibDirWithPostCopy(miniaudioDir, engineDir)
	LibDirGLFWPostCopy(_ACTION, engineDir)
	
	-- Linked libraries
	LinkLua()
	LinkGLFW()
	LinkOpenGL()
	LinkMiniaudio()
	LinkAssimp("lib", engineDir)

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
		"vendor/mono/include",
		"vendor"
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
	-- LibDirWithPostCopy("%{prj.name}/lib", playerDir)
	LibDirGLFWPostCopy(_ACTION, editorDir)

	-- Linked libraries
	links {
		engineName
--		"mono-2.0-sgen"
	}
	LinkGLFW(_ACTION, editorDir)
	LinkAssimp("lib", editorDir)

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
	-- LibDirWithPostCopy("%{prj.name}/lib", playerDir)

	-- Linked libraries
	LinkAssimp("lib", playerDir)
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