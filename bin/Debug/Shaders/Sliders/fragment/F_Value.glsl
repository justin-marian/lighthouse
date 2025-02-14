#version 330

// Output fragment color
out vec4 fragColor;

// Screen resolution
uniform vec2 u_resolution;
// Input color RGB<->HSV
uniform vec3 u_color;

// White & Black colors
const vec3 white = vec3(1.0);
const vec3 black = vec3(0.0);

void main()
{
    // Calculate horizontal position (resizable resolution)
    float position = gl_FragCoord.x / u_resolution.x;
    vec3 color;

    // Linear gradient
    if (position < 0.5)
    {
        // Mix black to color
        color = mix(black, u_color, position * 2.0);
    }
    else 
    {
        // Mix color to white
        color = mix(u_color, white, (position - 0.5) * 2.0);
    }

    fragColor = vec4(color, 1.0);
}
