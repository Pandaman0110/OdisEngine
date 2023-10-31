#version 460 core

out vec4 color;
uniform vec4 shape_color;

void main()
{
    color = vec4(shape_color);
} 