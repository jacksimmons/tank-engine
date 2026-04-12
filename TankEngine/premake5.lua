require("premake.outdir")


local wks = "%{wks.location}/"
local engineBin = wks .. ThisBinDir()
local editorBin = wks .. PrjBinDir("TankEditor")
local playerBin = wks .. PrjBinDir("TankPlayer")


project "TankEngine"
	kind "SharedLib"
	PrjUseCpp()
	PrjObjAndTargetDir()

	defines {
		"TANK_DLL",
		"TANK_ENGINEDIR=\"" .. engineBin .. "\"",
		"TANK_PLAYERDIR=\"" .. playerBin .. "\"",
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"GLAD_GLAPI_EXPORT",
		"GLAD_GLAPI_EXPORT_BUILD",
		"STBI_NO_SIMD",
	}

	includedirs {
		"", -- for pch
		"include",
		"src",
		wks .. "include",
		wks .. "include/glm",
		wks .. "include/imgui",
		wks .. "include/imgui/backends",
		wks .. "include/sol2/include",
		wks .. "include/lua",
		wks .. "vendor",
	}

	files {
		wks .. "include/imgui/imgui*.cpp",
		wks .. "include/imgui/backends/imgui_impl_glfw.cpp",
		wks .. "include/imgui/backends/imgui_impl_opengl3.cpp",
		wks .. "include/glad/glad.cpp",
		"src/**.hpp",
		"src/**.cpp"
	}

	-- Library dirs
	libdirs {
		wks .. "lib"
	}
	LibDirWithPostCopy(PrjBinDir("Lua"), engineBin)
	LibDirWithPostCopy(PrjBinDir("miniaudio"), engineBin)
	LibDirGLFWPostCopy(wks .. "lib/", _ACTION, engineBin)
	
	-- Linked libraries
	links {
		"Lua",
		"miniaudio"
	}
	LinkGLFW()
	LinkOpenGL()
	LinkAssimp(wks .. "lib", engineBin)

	-- PCH
	pchheader "tepch.hpp"
	pchsource "src/application.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }
		
	-- Copy bin and libs into all applicable outdirs
	PostCopyDir(engineBin, editorBin)
	PostCopyDir(engineBin, playerBin)