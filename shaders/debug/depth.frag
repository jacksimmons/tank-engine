#version 460 core


out vec4 frag_colour;


// Outputs depth as greyscale fragment colour (white = far, black = near).

// Will be mostly white until you get very close to the object, due to
// depth precision (high precision at short distances, low precision
// at far distances).
void main()
{
    frag_colour = vec4(vec3(gl_FragCoord.z), 1.0);
}