---@meta
---@enum KeyCode_t
KeyCode = {
    W = nil,
    A = nil,
    S = nil,
    D = nil
}

---@class KeyState_t
---@field Pressed number
---@field Held number
---@field Released number
---@field NotPressed number
---@type KeyState_t
KeyState = {}

---@class KeyInput_t
KeyInput = {}

---@param key_code KeyCode_t
---@return KeyState_t
function KeyInput:get_key_state(key_code) end