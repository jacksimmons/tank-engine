local subDir = "%{prj.name}/%{cfg.shortname}"
local groupSubDir = "%{prj.group}/%{prj.name}/%{cfg.shortname}"

---comment Sets project language to c
function PrjUseC()
    language "C"
    cdialect "c17"
end

---comment Sets project language to c++
function PrjUseCpp()
    language "C++"
	cppdialect "c++23"
end

---comment Sets project language to C#
function PrjUseCSharp()
    language "C#"
    dotnetframework "net10.0"
end

function PrjObjAndTargetDir()
    targetdir ("%{wks.location}/Builds/bin/" .. subDir)
	objdir ("%{wks.location}/Builds/obj/" .. subDir)
end

function GrpPrjObjAndTargetDir()
    targetdir ("Builds/bin/" .. groupSubDir)
    objdir ("Builds/obj/" .. groupSubDir)
end