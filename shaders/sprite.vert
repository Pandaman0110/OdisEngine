#version 460 core
layout (location = 0) in vec4 vertex;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 projection;

//for instancing
//uniform vec2 offsets[1024];

void main()
{
    //vec2 offset = offsets[gl_InstanceID];
    tex_coords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}