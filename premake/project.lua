function PrjUseCpp()
    language "C++"
	cppdialect "c++23"
end

function PrjObjAndTargetDir()
    targetdir "Builds/bin/%{prj.name}/%{cfg.longname}"
	objdir "Builds/obj/%{prj.name}/%{cfg.longname}"
end