require("premake.links")
require("premake.outdir")


local wks = "%{wks.location}/"
local nfd = wks .. "include/nativefiledialog/"


project "TankEditor"
	kind "ConsoleApp"
	PrjUseCpp()
	PrjObjAndTargetDir()

	defines {
		"GLM_ENABLE_EXPERIMENTAL",
		"FMT_UNICODE=0",
		"GLAD_GLAPI_EXPORT",
	}

	includedirs {
		wks .. "include",
		wks .. "include/glm",
		wks .. "include/imgui",
		wks .. "include/sol2/include",
		wks .. "include/lua",
		nfd .. "src/include",
		"include",
		"src",
		wks .. "TankEngine/src",
		wks .. "TankEngine", -- for pch
		wks .. "vendor",
		wks .. "vendor/mono/include"
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
	-- LibDirWithPostCopy("%{prj.name}/lib", playerDir)
	LibDirWithPostCopy(wks .. "include/nativefiledialog/build/obj/x64/%{cfg.buildcfg}", GrpPrjBinDir())
	LibDirWithPostCopy(wks .. "include/nativefiledialog/build/lib/%{cfg.buildcfg}/x64", GrpPrjBinDir())
	LibDirGLFWPostCopy(_ACTION, GrpPrjBinDir())

	-- Linked libraries
	links {
		"TankEngine",
		"nfd_d",
		"gtk-3",
		"gdk-3",
		"glib-2.0",
		"gobject-2.0",
		"gio-2.0"
--		"mono-2.0-sgen"
	}
	LinkGLFW(_ACTION)
	LinkAssimp(wks .. "lib", GrpPrjBinDir())

	-- PCH
	pchheader "tepch.hpp"
	pchsource "src/editor.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }