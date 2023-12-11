#version 460 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in mat4 i_matrix;
layout (location = 5) in vec4 i_color;

out vec4 shape_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * i_matrix * vec4(vertex, 0.0, 1.0);
    shape_color = i_color;
}