require "premake.links"
require "premake.outdir"
require "premake.project"


local wks = "%{wks.location}/"
local nfd = wks .. "include/nativefiledialog/"
local binDir = wks .. PrjBinDir("TankEditor")


project "TankEditor"
	kind "ConsoleApp"
	PrjUseCpp()
	PrjObjAndTargetDir()

	defines {
		"TANK_WKS=\"" .. wks .. "\"",
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"GLAD_GLAPI_EXPORT",
	}

	includedirs {
		"include",
		"src",
		wks .. "include",
		wks .. "include/glm",
		wks .. "include/imgui",
		wks .. "include/sol2/include",
		wks .. "include/lua",
		wks .. "TankEngine/src",
		wks .. "TankEngine", -- for pch
		wks .. "vendor",
		wks .. "vendor/mono/include",
		nfd .. "src/include",
	}

	files {
		wks .. "include/imgui/imgui*.cpp",
		"src/**.hpp",
		"src/**.cpp",
	}

	-- Library dirs
	libdirs {
		wks .. "lib",
	}
	LibDirWithPostCopy(wks .. "include/nativefiledialog/build/obj/x64/%{cfg.buildcfg}", binDir)
	LibDirWithPostCopy(wks .. "include/nativefiledialog/build/lib/%{cfg.buildcfg}/x64", binDir)
	LibDirGLFWPostCopy(wks .. "lib/", _ACTION, binDir)

	-- Linked libraries
	filter { "system:linux" }
		links {
			"gtk-3",
			"gdk-3",
			"glib-2.0",
			"gobject-2.0",
			"gio-2.0"
		}
	filter { }
	links {
		"TankEngine",
		"nfd_d",
--		"mono-2.0-sgen"
	}

	LinkGLFW(_ACTION)
	LinkAssimp(wks .. "lib", binDir)

	-- PCH
	pchheader "tepch.hpp"
	pchsource "src/editor.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

	-- Visual Studio debugging
	debugdir "%{wks.location}"