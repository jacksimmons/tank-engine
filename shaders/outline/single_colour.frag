#version 430 core
uniform vec4 outline_col;
out vec4 frag_col;


void main()
{
    frag_col = outline_col;
}