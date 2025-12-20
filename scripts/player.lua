-- local vec4 = require("tank.vec4")
-- local node = require("tank.node")


function update()
    local vec3 = node.transform.translation

    if (node.key_input:get_key_state(KeyCode.W) == KeyState.Held) then
        vec3.z = vec3.z + 1
    elseif (node.key_input:get_key_state(KeyCode.S) == KeyState.Held) then
        vec3.z = vec3.z - 1

    -- Model is rotated 180 on z-axis
    elseif (node.key_input:get_key_state(KeyCode.A) == KeyState.Held) then
        vec3.x = vec3.x + 1
    elseif (node.key_input:get_key_state(KeyCode.D) == KeyState.Held) then
        vec3.x = vec3.x - 1
    end

    node.transform.translation = vec3
end