---@meta
---@class Vec3_t
---@field x number
---@field y number
---@field z number
Vec3 = {}

---Make a 3D zero vector.
---@return Vec3_t
function Vec3:new() end


---Make a new 3D vector.
---@param x number | nil
---@param y number | nil
---@param z number | nil
---@return Vec3_t
function Vec3:new(x, y, z) end

return Vec3;