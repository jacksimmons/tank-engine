local Transform = {}
function Transform.set_translation(transform, translation_vec3)
    transform.translation = translation_vec3
end
function Transform.set_rotation(transform, rotation_vec3)
    transform.rotation = rotation_vec3
end
function Transform.set_scale(transform, scale_vec3)
    transform.scale = scale_vec3
end

return Transform