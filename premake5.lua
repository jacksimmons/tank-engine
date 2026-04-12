require "os"
require "premake.project"
require "premake.command"
require "premake.links"
require "premake.outdir"


workspace "TankEngine"
    configurations { "Debug", "Release" }
	architecture "x86_64"

    filter { "configurations:Debug" }
	symbols "On"
    filter { "configurations:Release" }
	optimize "On"

	filter "system:linux"
        buildoptions { "-fPIC", "`pkg-config --cflags gtk+-3.0`" }
		linkoptions { "`pkg-config --libs gtk+-3.0`" }

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
	kind "StaticLib"
	PrjUseC()
	PrjObjAndTargetDir()

	includedirs {
		"include/lua"
	}

	files {
		"include/lua/*.c"
	}

project "miniaudio"
	kind "SharedLib"
	PrjUseC()
	PrjObjAndTargetDir()

	includedirs {
		"vendor"
	}

	files {
		"vendor/miniaudio/miniaudio.c"
	}

include "TankEngine"
include "TankEditor"
include "TankPlayer"


-- require "TankCSharpHost.premake5"
-- group "TankCSharpHost"
-- 	project "NativeHost"
-- 		NativeHostProject()
-- 	project "DotNetLib"
-- 		DotNetLibProject()
-- group ""