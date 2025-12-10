local subDir = "%{prj.name}/%{cfg.shortname}"
local groupSubDir = "%{prj.group}/%{prj.name}/%{cfg.shortname}"

function PrjUseC()
    language "C"
    cdialect "c23"
end


function PrjUseCpp()
    language "C++"
	cppdialect "c++23"
end

function PrjUseCSharp()
    language "C#"
    dotnetframework "net10.0"
end

function PrjObjAndTargetDir()
    targetdir ("Builds/bin/" .. subDir)
	objdir ("Builds/obj/" .. subDir)
end

function GrpPrjObjAndTargetDir()
    targetdir ("Builds/bin/" .. groupSubDir)
    objdir ("Builds/obj/" .. groupSubDir)
end