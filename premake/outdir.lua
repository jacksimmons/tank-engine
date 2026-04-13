---comment Returns the root bin dir
---@return string
function BinDir()
	return "Builds/bin/"
end

---comment Returns the bin dir for the current project
---@return string
function ThisBinDir()
	return BinDir() .. "%{prj.name}/%{cfg.shortname}"
end

---comment Returns the bin dir for a specific project
---@param project string
---@return string
function PrjBinDir(project)
	return BinDir() .. project .. "/%{cfg.shortname}"
end

-- -- Binary dir of a group
-- function GrpBinDir()
-- 	return BinDir() .. "%{prj.group}/"
-- end

-- -- Binary dir of particular project in a group
-- function GrpPrjBinDir()
-- 	return GrpBinDir() .. "%{prj.name}/"
-- end