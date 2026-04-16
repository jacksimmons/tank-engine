#version 430 core


float near = 1.0;
float far = 100;
out vec4 frag_colour;


float linearise_depth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}


// Outputs depth as greyscale fragment colour (white = far, black = near).

// Linearised so that depth is reflected more accurately, but doesn't show
// true value of depth (see depth.frag).
void main()
{
    float depth = linearise_depth(gl_FragCoord.z) / far;
    frag_colour = vec4(vec3(depth), 1.0);
}