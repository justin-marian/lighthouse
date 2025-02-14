#version 330

// Input vertex position
layout (location = 0) in vec2 v_position;

// Transformation matrixes
uniform mat4 Model;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * Model * vec4(v_position, 0.0, 1.0);
}
