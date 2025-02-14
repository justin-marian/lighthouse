#version 330

// https://learnopengl.com/Lighting/Multiple-lights

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 texCoords;
uniform vec3 object_color;
uniform float angle;

uniform vec3 light_position[15];         // 4 BOATS + 2 LIGHTHOUSE + 1 MOON + 8 BASE LIGHTHOUSE
uniform vec3 light_direction[15];        // 4 BOATS + 2 LIGHTHOUSE + 1 MOON + 8 BASE LIGHTHOUSE
uniform vec3 light_color[15];            // 4 BOATS + 2 LIGHTHOUSE + 1 MOON + 8 BASE LIGHTHOUSE

uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform float material_ka;
uniform vec3 material_ke;
uniform uint material_shininess;

// Uniform
uniform sampler2D textures[10];      // Array of textures, MAX = 10
uniform float mix_factors[9];        // Mix factors for blending textures
uniform int numTextures;             // The actual number of textures used
uniform vec3 objectColor;            // Object color if no textures are used

// Output
layout(location = 0) out vec4 out_color;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


vec3 PointLight(vec3 lightPos, vec3 lightColor, vec3 fragPos, vec3 normal)
{
    vec3 L = normalize(lightPos - fragPos);
    vec3 V = normalize(eye_position - fragPos);
    vec3 R = reflect(-L, normal);

    float N = dot(normal, L);
    // No light if the surface is facing away
    if (N < 0.0)
    {
        return vec3(0.0);
    }

    float d = max(N, 0.0);
    float s = pow(max(dot(V, R), 0.0), material_shininess); 

    // Reduced attenuation impact for more intense light
    float dist = length(lightPos - fragPos);
    float a = 1.0 / (1.0 + 0.2 * dist + 0.1 * dist * dist);

    // Boosted material properties for more reflectivity - X2
    vec3 A = material_ka * lightColor;
    vec3 D = material_kd * 2.0 * d * lightColor;
    vec3 S = material_ks * 2.0 * s * lightColor;

    return (A + a * (D + S));
}


vec3 SpotLight(vec3 lightPos, vec3 lightDir, vec3 lightColor, vec3 fragPos, vec3 normal, float cutoffAngle)
{
    vec3 L = normalize(fragPos - lightPos);
    vec3 V = normalize(eye_position - fragPos);
    vec3 R = reflect(L, normal);
    
    lightDir = normalize(-lightDir);
    float theta = dot(L, lightDir);

    if (theta > cos(cutoffAngle))
    {
        // Attenuation coefficients
        float c = 1.0;   // Constant
        float l = 0.7;   // Linear
        float q = 1.8;   // Quadratic

        float dist = length(lightPos - fragPos);
        float a = 1.0 / (c + l * dist + q * dist * dist);

        float d = max(dot(normal, -L), 0.0);
        float s = pow(max(dot(V, R), 0.0), material_shininess);

        // Amplify light color - increase intensity of the color
        float intensity = smoothstep(cos(cutoffAngle), cos(cutoffAngle), theta);
        lightColor *= intensity * 5;

        // Boosted material properties for more reflectivity - X3,X5
        vec3 A = material_ka * lightColor;
        vec3 D = material_kd * 3.0 * d * lightColor; 
        vec3 S = material_ks * 5.0 * s * lightColor;
        vec3 E = material_ke;

        return (A + a * (D + S) + E);
    }
    else
    {
        // Outside of spotlight cone
        return vec3(0.0);
    }
}


vec3 DirectionalLight(vec3 lightDir, vec3 lightColor, vec3 fragPos, vec3 normal)
{
    vec3 L = normalize(lightDir);
    vec3 V = normalize(eye_position - fragPos);
    vec3 R = reflect(-L, normal);

    float N = dot(normal, -L);

    // Soften the diffuse component
    float d = max(N, 0.0);
    float s = pow(max(dot(V, R), 0.0), material_shininess);

    // Calculate ambient, diffuse, and specular components
    vec3 A = material_ka * lightColor;
    vec3 D = material_kd * d * lightColor;
    vec3 S = material_ks * s * lightColor;
    vec3 E = material_ke * lightColor;

    return (A + D + S + E);
}


void main()
{
    vec3 resultLight = vec3(0.0);

    // Process each light
    // Boats - Point Lights i < 4
    // Lighthouses - Spotlights i >= 4 & i <=5
    // Moon - Directional Light i == 6
    // Lighthouse Ground lights - Point Lights i >= 7

    for (int i = 0; i < 15; i++)
    {
        if (i < 4)
        {
           resultLight += PointLight(light_position[i], light_color[i], world_position, world_normal);
        }
        else if (i >= 4 && i <= 5)
        {
           resultLight += SpotLight(light_position[i], light_direction[i], light_color[i], world_position, world_normal, angle);
        }
        else if (i == 6)
        {
          resultLight += DirectionalLight(light_direction[i], light_color[i], world_position, world_normal);
        }
        else if (i >= 7)
        {
            resultLight += PointLight(light_position[i], light_color[i], world_position, world_normal);
        }
    }

    vec4 finalColor;

    // Apply textures (if any) or use object color
    if (numTextures > 0)
    {
        vec4 mixedColor = texture(textures[0], texCoords) * mix_factors[0];
        for (int i = 1; i < numTextures; ++i)
        {
            mixedColor += texture(textures[i], texCoords) * mix_factors[i];
        }
        finalColor = mixedColor * vec4(resultLight, 1.0);
    }
    else
    {
        finalColor = vec4(objectColor * resultLight, 1.0);
    }

    finalColor.rgb *= resultLight;
    if (finalColor.a < 0.1)
    {
        discard;
    }
    out_color = finalColor;
}
