#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

/// HEIGHTMAP TEXTURE
uniform sampler2D heightMap;

// Output
out vec2 texCoords;
out float vertex_height; 

out vec3 world_position; 
out vec3 world_normal;  

// e^ix = cosx + i * sinx
vec2 complexExp(float x)
{
    return vec2(cos(x), sin(x));
}

// https://www.researchgate.net/publication/264839743_Simulating_Ocean_Water

// Gerstner waves with smoother motion
vec3 GerstnerWave(vec3 position, float time)
{
    // Approximative values to generate waves
    float waveLength = 4.0;
    float waveHeight = 0.05;
    vec2 direction = normalize(vec2(1.0, 1.0));

    float k = 2.0 * 3.141592 / waveLength;
    float w = sqrt(9.81 * k);
    float phi = w * 0.25;

    float c = cos(k * dot(direction, position.xz) - w * time + phi);
    float s = sin(k * dot(direction, position.xz) - w * time + phi);

    vec3 displacement;
    displacement.x = direction.x * waveHeight * c;
    displacement.y = waveHeight * s;
    displacement.z = direction.y * waveHeight * c;

    return position + displacement;
}

/// https://en.wikipedia.org/wiki/Discrete_Fourier_transform

// Discrete Fourier Transform - smoother waves
vec3 DFTWave(vec3 position, vec2 texCoord, float t)
{
    const int N = 16;
    vec3 waveOffset = vec3(0.0);

    for (int n = 0; n < N; ++n)
    {
        float freq = float(n) * 0.15;
        vec2 cexp = complexExp(-2.0 * 3.141592 * freq * (position.x + position.z) + t * freq);

        waveOffset.x += cexp.x * 0.005;
        waveOffset.y += cexp.y * 0.0125;
        waveOffset.z += cexp.x * 0.005;
    }

    return position + waveOffset;
}

// Combine Gerstner wave and DFT wave transforms
vec3 combinedWaveTransform(vec3 position, vec2 texCoord, float t)
{
    position = GerstnerWave(position, t);
    position = DFTWave(position, texCoord, t);
    return position;
}

void main()
{
    texCoords = v_texture_coord;
    vertex_height = texture(heightMap, texCoords).r;

    float scaleX = 2.0;
    float scaleZ = 2.0;

    vec3 scaledPosition = v_position;

    scaledPosition.x *= scaleX;
    scaledPosition.z *= scaleZ;
    
    vec3 newPosition = scaledPosition + vec3(0.0, vertex_height * 5, 0.0);

    // Apply wave transform based on water threshold
    if (vertex_height < 0.1)
    {
        newPosition = combinedWaveTransform(newPosition, texCoords, time);
    }

    // Compute world position and normal
    world_position = (Model * vec4(newPosition, 1)).xyz;
	world_normal = normalize( mat3(Model) * normalize(v_normal) );

    gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
}
