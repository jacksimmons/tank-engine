require("premake.outdir")


local playerBin = ThisBinDir()


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
		"include/sol2/include",
		"include/lua",
		"%{prj.name}/include",
		"%{prj.name}/src",
		"TankEngine/src",
		"TankEngine", -- for pch
	}

	-- Library dirs
	libdirs {
		"lib",
	}
	-- LibDirWithPostCopy("%{prj.name}/lib", playerDir)

	-- Linked libraries
	LinkAssimp("lib", playerBin)
	links {
		"TankEngine"
	}

	-- PCH
	pchheader "tepch.hpp"
	pchsource "%{prj.name}/src/player.cpp"
	filter { "action:vs*" }
		buildoptions { "/FI tepch.hpp" }