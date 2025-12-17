local os_type = {}

function os_type.windows()
    local fileSeparator = package.config:sub(1,1)
    if fileSeparator == "\\" then
        return true
    elseif fileSeparator == "/" then
        return false
    else
        error("FileSep was " .. fileSeparator .. " - not sure what this OS is.")
    end
end

return os_type