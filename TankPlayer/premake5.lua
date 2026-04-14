require("premake.outdir")


local wks = "%{wks.location}/"
local playerBin = wks .. PrjBinDir("%{prj.name}")
local playerReleaseBin = wks .. "%{prj.name}/ReleaseBuilds/%{cfg.system}/"


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
		"src/**.hpp",
		"src/**.cpp",
	}

	includedirs {
		"src",
		wks .. "include",
		wks .. "include/glm",
		wks .. "include/sol2/include",
		wks .. "include/lua",
		wks .. "TankEngine/src",
		wks .. "TankEngine", -- for pch
	}

	-- Library dirs
	libdirs {
		wks .. "lib",
	}
	-- LibDirWithPostCopy("%{prj.name}/lib", playerDir)

	-- Linked libraries
	LinkAssimp(wks .. "lib", playerBin)
	links {
		"TankEngine"
	}

	-- PCH
	pchheader "tepch.hpp"
	pchsource (wks .. "%{prj.name}/src/player.cpp")
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }

	-- Copy to non gitignored folder if in Release mode
	filter { "configurations:Release" }
		PostCopyDir(playerBin, playerReleaseBin)
	filter {}