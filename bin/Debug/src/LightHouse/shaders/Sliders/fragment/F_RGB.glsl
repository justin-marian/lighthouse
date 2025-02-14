#version 330

// Input object color
uniform vec3 objectColor;

// Output fragment color
out vec4 fragColor;

void main()
{
    fragColor = vec4(objectColor, 1.0);
}
