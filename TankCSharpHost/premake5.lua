require "premake.command"
require "premake.links"
require "premake.outdir"


function NativeHostProject()
	local dest = GrpPrjBinDir() .. "%{cfg.longname}"

    kind "SharedLib"
    PrjUseCpp()
    GrpPrjObjAndTargetDir()

    includedirs {
        "%{prj.group}/include",
    }

    files {
        "%{prj.group}/include/*.h",
        "%{prj.group}/%{prj.name}/src/*.cpp"
    }

    LibDirWithPostCopy("%{prj.group}/lib/dotnet10", dest)
    links {
        "nethost"
    }
end


function DotNetLibProject()
	local dest = GrpPrjBinDir() .. "%{cfg.longname}"

    kind "SharedLib"
    PrjUseCSharp()
    GrpPrjObjAndTargetDir()

    files {
        "%{prj.group}/%{prj.name}/*.cs"
    }
end