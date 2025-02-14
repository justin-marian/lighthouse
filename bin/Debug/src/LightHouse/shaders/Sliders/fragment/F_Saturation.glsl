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

    // Smooth gradient
    if (position < 0.75)
    {
        // Smooth interpolation
        float interpFactor = smoothstep(0.0, 0.5, position);
        // Mix black to color
        color = mix(black, u_color, interpFactor);
    }
    else 
    {
        // Smooth interpolation
        float interpFactor = smoothstep(0.5, 1.0, position);
        // Mix color to white
        color = mix(u_color, white, interpFactor);
    }

    fragColor = vec4(color, 1.0);
}
