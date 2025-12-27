function Update()
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

    local pos = node.transform.translation
    pos = pos + displace

    local cam = Scene:current():active_camera()
    cam:set_pos(pos - Vec3:new(10, -10, 30))

    node.transform.translation = pos
end