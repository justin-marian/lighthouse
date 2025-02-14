#version 330

// Input from vertex shader
in vec2 texcoord;
uniform vec3 objectColor;

// Uniforms
uniform sampler2D textures[10];
// MAX = 10 (it can support maximum 10 texture)
uniform int numTextures;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    if (numTextures > 0)
    {
        vec4 mixedColor = texture(textures[0], texcoord);
        for (int i = 1; i < numTextures; ++i)
        {
            mixedColor += texture(textures[i], texcoord);
        }
        mixedColor /= float(numTextures);
        out_color = mixedColor;
    }
    else
    {
        out_color = vec4(objectColor, 1.0);
    }
}