#version 460 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 coords;

out vec2 tex_coords;

//uniform mat4 model;
//uniform mat4 projection;
//uniform mat4 view;

void main()
{
    gl_Position = vec4(pos.xy, 0.0, 1.0); 
    tex_coords = coords;
}