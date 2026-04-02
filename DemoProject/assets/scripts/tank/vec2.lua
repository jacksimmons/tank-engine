local vec2 = {}

--- @param x number First component.
--- @param y number Second component.
function vec2.new(x, y)
    return setmetatable({x = x or 0, y = y or 0}, vec2)
end

return vec2