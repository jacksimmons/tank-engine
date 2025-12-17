#version 430 core
// Direction which represents a 3D texture coordinate
in vec3 tex_coords;
uniform samplerCube cubeMap;
out vec4 fragColour;


void main(void)
{
	fragColour = texture(cubeMap, tex_coords);
}