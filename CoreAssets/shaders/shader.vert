#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

out vec3 frag_normal;
out vec3 frag_pos;
out vec2 frag_tex_coords;

uniform mat4 PVM;
uniform mat4 VM;
uniform mat4 VM_it;

void main()
{
    gl_Position = PVM * vec4(position, 1.0);

    frag_pos = vec3(VM * vec4(position, 1.0f));
    frag_normal = vec3(VM_it * vec4(normalize(normal), 1.0f));
    frag_tex_coords = tex_coords;
}
