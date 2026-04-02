local vec3 = {}

--- @param x number First component.
--- @param y number Second component.
--- @param z number Third component.
function vec3.new(x, y, z)
    return setmetatable({x = x or 0, y = y or 0, z = z or 0}, vec3)
end

return vec3