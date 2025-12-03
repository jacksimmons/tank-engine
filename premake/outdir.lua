-- Binary dir
function BinDir()
	return "Builds/bin/"
end

-- Binary dir of a group
function GrpBinDir()
	return BinDir() .. "%{prj.group}/"
end

-- Binary dir of particular project in a group
function GrpPrjBinDir()
	return GrpBinDir() .. "%{prj.name}/"
end