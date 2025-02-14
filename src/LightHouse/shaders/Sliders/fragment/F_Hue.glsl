#version 330

// Input factor color
in float factor;

// Output fragment color
out vec4 fragColor;

vec3 hsv2rgb(vec3 c) 
{
    // Convert HSV color to RGB color - rainbow 6 sectors effect

    // constants for color conversion
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);

    //      p based on the hue value (c.x) and constants in K
    // Creates repeating pattern as hue changes, scaled and mirrored
    //              it can fit within [0,1]
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);

    //      c.z - represents the value (brightness)
    // Mixes colors and clamps them to create the final RGB color
    //              it can fit within [0,1]
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    // Fully saturated and has full brightness
    vec3 hueColor = vec3(factor, 1.0, 1.0);
    // Convert HSV->RGB channel format
    fragColor = vec4(hsv2rgb(hueColor), 1.0);
}
