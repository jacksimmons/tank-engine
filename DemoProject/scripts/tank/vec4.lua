local vec4 = {}

--- @param x number First component.
--- @param y number Second component.
--- @param z number Third component.
--- @param w number Fourth component.
function vec4.new(x, y, z, w)
    return setmetatable({x = x or 0, y = y or 0, z = z or 0, w = w or 0}, vec4)
end

return vec4