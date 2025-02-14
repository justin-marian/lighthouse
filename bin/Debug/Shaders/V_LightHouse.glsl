#version 330

layout(location = 0) in vec3 v_position;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 texcoord;

void main()
{
    texcoord = v_texture_coord;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
