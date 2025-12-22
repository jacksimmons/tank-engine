function Update()
    local pos = node.transform.translation
    local displace = Vec3:new()

    if (node.key_input:get_key_state(KeyCode.W) == KeyState.Held) then
        displace.z = 1
    end
    if (node.key_input:get_key_state(KeyCode.S) == KeyState.Held) then
        displace.z = -1
    end

    -- Model is rotated 180 on z-axis
    if (node.key_input:get_key_state(KeyCode.A) == KeyState.Held) then
        displace.x = 1
    end
    if (node.key_input:get_key_state(KeyCode.D) == KeyState.Held) then
        displace.x = -1
    end

    pos.x = pos.x + displace.x
    pos.y = pos.y + displace.y
    pos.z = pos.z + displace.z
    camera:set_pos(Vec3:new(pos.x - 10, pos.y + 10, pos.z - 30))
    node.transform.translation = pos
end