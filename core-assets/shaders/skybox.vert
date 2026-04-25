#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 proj;
uniform mat4 view;
out vec3 tex_coords;


void main()
{
	gl_Position = proj * view * vec4(position, 1);
    gl_Position.z = gl_Position.w * 0.999;
	tex_coords = -position;
}
