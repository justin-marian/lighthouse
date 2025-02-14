#include "LightHouse.h"

#include "GTexture.h"
#include "GMesh.h"
#include "GShader.h"

#include "SliderManager.h"

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <algorithm>


using namespace std;


LightHouse::LightHouse() :
    /// LOADING SHADERS+TEXTUERS+MESHES
    gameInit(new GameInit(meshes, shaders, textures)),  // GameInit
    sliderManager(new SliderManager()),                 // SliderManager
    elapsedTime(Engine::GetElapsedTime()),              // Time elapsed since the start of the engine
    lighthousePosition(glm::vec3(0, 1, 0)),             // Position of the lighthouse in the scene
    /// LIGHT PROPERTIES
    materialShininess(0),
    materialKd(0.0f), materialKs(0.0f),
    materialKa(0.0f), materialKe(glm::vec3(0.0f)),
    angleCutOff(0.0f) {

    // Load resources and initialize game components
    gameInit->LoadResources();

    // Set up random distributions for boat properties
    std::random_device rd;
    std::mt19937 gen(rd());
    /// ANGLES, SPPED, DIRECTIONS OF BOATS
    std::uniform_real_distribution<> angleDist(0.0, 2 * M_PI);
    std::uniform_real_distribution<> speedDist(0.05, 0.2);
    std::uniform_int_distribution<> directionDist(0, 1);
    /// SPACE RANGE BETWEEN BOATS
    std::uniform_real_distribution<> distanceDist1(5.0, 7.5);
    std::uniform_real_distribution<> distanceDist2(8.0, 10.5);
    std::uniform_real_distribution<> distanceDist3(11.0, 13.5);
    std::uniform_real_distribution<> distanceDist4(14.0, 16.5);

    for (int i = 0; i < 4; i++)
    {
        boatRotationAngles[i] = angleDist(gen);
        boatRotationSpeeds[i] = speedDist(gen);
        boatRotationDirections[i] = directionDist(gen) ? 1 : -1;

        // Assign distance based on boat index
        switch (i) {
            case 0: radiusDist[i] = distanceDist1(gen); break;
            case 1: radiusDist[i] = distanceDist2(gen); break;
            case 2: radiusDist[i] = distanceDist3(gen); break;
            case 3: radiusDist[i] = distanceDist4(gen); break;
        }
    }

    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(0.5, 0, 0), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
        // Second row (middle)
        VertexFormat(glm::vec3(0, 0.5, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(0.5, 0.5, 0), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(1, 0.5, 0), glm::vec3(0, 0, 1)),
        // Third row (top)
        VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(0.5, 1, 0), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0, 0, 1))
    };

    vector<unsigned int> indices =
    {
        // First square
        0, 1, 4,
        0, 4, 3,
        // Second square
        1, 2, 5,
        1, 5, 4,
        // Third square
        3, 4, 7,
        3, 7, 6,
        // Fourth square
        4, 5, 8,
        4, 8, 7
    };

    gameInit->CreateMesh("slider", vertices, indices);
}


LightHouse::~LightHouse()
{ 
    delete gameInit;
    delete sliderManager;
}


void LightHouse::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resolution = window->GetResolution();
    windowWidth = resolution.x;
    windowHeight = resolution.y;
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void LightHouse::FrameEnd()
{
    DrawCoordinateSystem();
}


void LightHouse::Init()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    boatInitialColors.resize(4);

    for (int i = 0; i < 4; ++i)
    {
        boatInitialColors[i] = glm::vec3(
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
        );
    }

    // Light & material properties
    {
        materialShininess = 30;
        angleCutOff = RADIANS(45);
        materialKd = 0.5;
        materialKs = 0.5;
        materialKa = 0.01;
        materialKe = glm::vec3(0.1f);
    }
}


/// <summary>
/// Render the lighthouse object in the scene
/// Set up textures and positions for the lighthouse components. 
/// </summary>
void LightHouse::RenderLighthouseObject()
{
    std::vector<Texture2D*> baseHouseTextures = { textures["base-house"] };
    std::vector<Texture2D*> lighthouseMidTextures = { textures["mid-house"] };
    std::vector<Texture2D*> lighthouseTextures = { textures["top-house"] };

    // Render the base of the lighthouse
    glm::mat4 baseModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0.0, 0));
    baseModelMatrix = glm::scale(baseModelMatrix, glm::vec3(3.f, 1.5f, 3.f));
    RenderTextured(meshes["sphere"], shaders["Scene"], baseModelMatrix, baseHouseTextures);

    // Setup lighting around the lighthouse
    SetupLighthouseLighting();

    // Render the middle of the lighthouse
    glm::mat4 middleModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0.5, 0));
    middleModelMatrix = glm::scale(middleModelMatrix, glm::vec3(2.45f, 10.0f, 2.45f));
    RenderTextured(meshes["lighthouse"], shaders["Scene"], middleModelMatrix, lighthouseMidTextures);

    // Render the lower layer (balcony-like structure)
    glm::mat4 lowerLayerModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 10.5, 0));
    lowerLayerModelMatrix = glm::scale(lowerLayerModelMatrix, glm::vec3(3.0f, 0.25f, 3.0f));
    RenderTextured(meshes["lighthouse"], shaders["Scene"], lowerLayerModelMatrix, lighthouseTextures);

    // Update light positions and render the upper layer with rotating lights
    UpdateAndRenderUpperLayer();

    // Render the top of the lighthouse
    glm::mat4 topModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 12.f, 0));
    topModelMatrix = glm::scale(topModelMatrix, glm::vec3(3.0f, 0.25f, 3.0f));
    RenderTextured(meshes["lighthouse"], shaders["Scene"], topModelMatrix, lighthouseTextures);
}


/// <summary>
/// Render boats in the scene
/// Responsible for animating and displaying boats around the lighthouse.
/// </summary>
/// <param name="deltaTimeSeconds">Time elapsed since the last frame, used for animations.</param>
void LightHouse::RenderBoats(float deltaTimeSeconds)
{
    std::vector<Texture2D*> boatCombo1 = { textures["wood1"], textures["wood3"], textures["iron_dark"] };
    std::vector<Texture2D*> boatCombo2 = { textures["wood1"], textures["wood2"], textures["iron_dark"] };
    std::vector<Texture2D*> boatCombo3 = { textures["wood1"], textures["wood3"], textures["iron_rust"] };
    std::vector<Texture2D*> boatCombo4 = { textures["wood1"], textures["wood2"], textures["iron_rust"] };
    std::vector<std::vector<Texture2D*>> boatCombos = { boatCombo1, boatCombo2, boatCombo3, boatCombo4 };
    std::vector<float> mixFactorsBoats = { 0.5f, 0.5f };

    for (int i = 0; i <= 3; i++)
    {
        // Update rotation angle for circular motion around the lighthouse
        boatRotationAngles[i] += deltaTimeSeconds * boatRotationSpeeds[i] * boatRotationDirections[i];

        // Calculate new position for the boat
        float x = lighthousePosition.x + radiusDist[i] * cos(boatRotationAngles[i]);
        float z = lighthousePosition.z + radiusDist[i] * sin(boatRotationAngles[i]);
        glm::vec3 boatPosition = glm::vec3(x, 0.5, z);

        // Update the corresponding point light position
        glm::vec3 lightPositionUnderBoat = glm::vec3(boatPosition.x, 1.0f, boatPosition.z);
        point_light_pos[i] = lightPositionUnderBoat;
        point_light_dir[i] = glm::vec3(0, -1, 0);
        point_light_color[i] = boatInitialColors[i];

        // Set up model matrix for the boat
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, boatPosition);
        float directionAngle = atan2(-z, x) + M_PI / 2.0f;
        modelMatrix = glm::rotate(modelMatrix, directionAngle, glm::vec3(0, 1, 0));

        if (boatRotationDirections[i] < 0)
        {
            modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
        }

        modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f));

        RenderTextured(meshes["wake_boat"], shaders["Scene"], modelMatrix, boatCombos[i], mixFactorsBoats);
    }
}


/// <summary>
/// Render the moon in the scene
/// Position and textures the moon, and sets up its lighting.
/// </summary>
void LightHouse::RenderMoon()
{
    float moonSpeed = 0.1f;
    float moonOrbitRadius = 200.0f;
    std::vector<Texture2D*> moonTextures = { textures["moonHMap"] };

    // Calculate the moon's position in a circular orbit
    glm::vec3 position = glm::vec3(
        moonOrbitRadius * cos(elapsedTime * moonSpeed),
        40.0f,
        moonOrbitRadius * sin(elapsedTime * moonSpeed)
    );

    point_light_pos[6] = position;
    point_light_color[6] = glm::vec3(1.f);

    // Define downward direction for moonlight
    glm::vec3 moonLightDirection = glm::normalize(position);
    point_light_dir[6] = moonLightDirection;

    // Update model matrix
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(10.f));

    // Tidal locking: Axial rotation matches orbital speed
    float axialRotationSpeed = moonSpeed;
    modelMatrix = glm::rotate(modelMatrix, elapsedTime * axialRotationSpeed, glm::vec3(0.0f, -1.0f, 0.0f));

    RenderTextured(meshes["sphere"], shaders["LightHouse"], modelMatrix, moonTextures);
}


/// <summary>
/// Render the lake plane in the scene
/// Set up the lake's textures and position.
/// </summary>
void LightHouse::RenderLakePlane()
{
    std::vector<Texture2D*> mountainTexture = { textures["groundHMap"], textures["ground"],
                                                textures["water"], textures["waterUV"],
                                                textures["lava"], textures["lavaUV"], textures["lavaOcc"] };
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75));

    RenderTextured(meshes["lake"], shaders["Lake"], modelMatrix, mountainTexture);
}


/// <summary>
/// Set up lighting around the lighthouse
/// Configure light positions and colors for the lighthouse.
/// </summary>
void LightHouse::SetupLighthouseLighting()
{
    const int numLights = 8;
    const float radius = 3.0f;
    const float Y_height = 3.0f;

    for (int i = 7; i < 15; ++i)
    {
        float angle = 2.0f * M_PI * i / numLights;
        point_light_pos[i] = glm::vec3(radius * cos(angle), Y_height, radius * sin(angle));
        point_light_dir[i] = glm::vec3(0.0f, -1.0f, 0.0f);
        point_light_color[i] = sliderManager->getLighthouseColor();
    }
}


/// <summary>
/// Update and renders the upper layer of the lighthouse
/// Handle the animation and lighting of the upper layer.
/// </summary>
void LightHouse::UpdateAndRenderUpperLayer()
{
    const float rotationSpeed = 1.0f;
    const float rotationRadius = 1.0f;
    const glm::vec3 upperLayerCenter(0.0f, 3.25f, 0.0f);

    for (int i = 4; i <= 5; ++i)
    {
        float rotationAngle = elapsedTime * rotationSpeed + (i == 5 ? M_PI : 0.0f);
        glm::vec3 position = upperLayerCenter + rotationRadius * glm::vec3(cos(rotationAngle), 0, sin(rotationAngle));

        point_light_pos[i] = position;
        point_light_dir[i] = glm::normalize(upperLayerCenter - position);
        point_light_color[i] = sliderManager->getLighthouseColor();
    }

    glm::mat4 upperLayerModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 10.5, 0));
    upperLayerModelMatrix = glm::scale(upperLayerModelMatrix, glm::vec3(2.5f, 1.5f, 2.5f));
    RenderTextured(meshes["lighthouse"], shaders["LightHouse"], upperLayerModelMatrix, {}, {}, sliderManager->getLighthouseColor());
}


/// <summary>
/// Render sliders for the UI
/// Draw sliders for user interaction, like color adjustment.
/// </summary>
void LightHouse::RenderSliders()
{
    const auto& sliders = sliderManager->getSliders();

    for (size_t i = 0; i < sliders.size(); ++i)
    {
        // Create model matrix based on slider position and size
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(sliders[i].position, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(sliders[i].size, 1.0f));

        // Determine which shader to use based on the slider index
        switch (i) {
        case 0: case 1: case 2: // RGB Sliders
            RenderSlider(meshes["slider"], shaders["RGB"], modelMatrix, sliders[i].color);
            break;
        case 3: // Hue Slider
            RenderSlider(meshes["slider"], shaders["HUE"], modelMatrix, sliders[i].color);
            break;
        case 4: // Saturation Slider
            RenderSlider(meshes["slider"], shaders["SAT"], modelMatrix, sliderManager->getLighthouseColor());
            break;
        case 5: // Value Slider
            RenderSlider(meshes["slider"], shaders["VAL"], modelMatrix, sliders[i].color);
            break;
        default: // Default case for unexpected index
            RenderSlider(meshes["slider"], shaders["RGB"], modelMatrix, sliders[i].color);
            break;
        }
    }
}


/// <summary>
/// Render a single slider
/// Helper method for drawing individual sliders.
/// </summary>
/// <param name="mesh">Mesh for the slider</param>
/// <param name="shader">Shader to use</param>
/// <param name="modelMatrix">Model matrix for positioning the slider</param>
/// <param name="color">Color of the slider</param>
void LightHouse::RenderSlider(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    RenderTextured(mesh, shader, modelMatrix, {}, {}, color, true);
}


/// <summary>
/// Render bamboo objects in the scene
/// Place and textures several bamboo objects around the lighthouse.
/// </summary>
void LightHouse::RenderBamboos()
{
    std::vector<glm::vec3> bambooPositions =
    {
        glm::vec3(-1.75, 0, -1.75),
        glm::vec3(1.75, 0, -1.75),
        glm::vec3(-1.75, 0, 1.75),
        glm::vec3(1.75, 0, 1.75),
        /// between each of the first 4 bamboos put other bamboos
        (glm::vec3(-1.75, 0, -1.75) + glm::vec3(1.75, 0, -1.75)) / 1.5f,
        (glm::vec3(1.75, 0, -1.75) + glm::vec3(1.75, 0, 1.75)) / 1.5f,
        (glm::vec3(-1.75, 0, 1.75) + glm::vec3(1.75, 0, 1.75)) / 1.5f,
        (glm::vec3(-1.75, 0, -1.75) + glm::vec3(-1.75, 0, 1.75)) / 1.5f
    };

    float bambooScale = 0.1f;
    std::vector<Texture2D*> bambooTextures = { textures["bamboo"] };

    for (const auto& pos : bambooPositions) {
        glm::mat4 bambooModelMatrix = glm::mat4(1);
        bambooModelMatrix = glm::translate(bambooModelMatrix, pos);
        bambooModelMatrix = glm::rotate(bambooModelMatrix, glm::radians(90.0f), glm::vec3(0, 1, 0));
        bambooModelMatrix = glm::scale(bambooModelMatrix, glm::vec3(bambooScale));
        RenderTextured(meshes["bamboo"], shaders["Scene"], bambooModelMatrix, bambooTextures);
    }
}


/// <summary>
/// Updates the scene based on elapsed time
/// Handles time-based updates like animations.
/// </summary>
/// <param name="deltaTimeSeconds">Time elapsed since the last frame.</param>
void LightHouse::Update(float deltaTimeSeconds)
{
    elapsedTime = Engine::GetElapsedTime();
    std::srand(std::time(nullptr));
    RenderBoats(deltaTimeSeconds);
    RenderLighthouseObject();
    RenderMoon();
    RenderLakePlane();
    RenderBamboos();
    RenderSliders();
}


/// <summary>
/// Render a textured object
/// General method for rendering any textured object in the scene.
/// </summary>
/// <param name="mesh">Mesh of the object</param>
/// <param name="shader">Shader to use</param>
/// <param name="modelMatrix">Model matrix for positioning</param>
/// <param name="textures">Textures to apply</param>
/// <param name="mixFactors">Factors for mixing textures</param>
/// <param name="color">Color of the object</param>
/// <param name="orthographic_perspective">Whether to use orthographic perspective</param>
void LightHouse::RenderTextured(
    Mesh* mesh,
    Shader* shader,
    // Model matrix applied with (T, R, S)
    const glm::mat4& modelMatrix,
    // Textures of the objects,
    // plus with an uniform mix of them applied on object
    std::vector<Texture2D*> textures,
    std::vector<float> mixFactors,
    // Color of the objects, in case if they dont use textures
    const glm::vec3& color,
    // Ortographic sliders, in rest perspective
    bool orthographic_perspective)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    SetupMatrices(shader, modelMatrix, orthographic_perspective);
    SetupSlider(shader, color);
    SetupLighting(shader, color);
    SetupTextures(shader, textures, mixFactors);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/// <summary>
/// Set up textures for rendering
/// Bind textures and sets shader uniforms for texture mixing.
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="textures">Textures to apply</param>
/// <param name="mixFactors">Factors for mixing textures</param>
void LightHouse::SetupTextures(
    Shader* shader,
    const std::vector<Texture2D*>& textures,
    std::vector<float>& mixFactors)
{
    int texturesBound = 0;
    float totalMixFactor = 0.0f;

    for (size_t i = 0; i < textures.size(); ++i)
    {
        if (textures[i])
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]->GetTextureID());

            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            glUniform1i(glGetUniformLocation(shader->program, ("textures[" + std::to_string(i) + "]").c_str()), i);
            texturesBound++;

            if (i < mixFactors.size()) {
                totalMixFactor += mixFactors[i];
            }
        }
    }

    if (totalMixFactor < 1.0f && texturesBound > 1)
    {
        for (size_t i = 0; i < mixFactors.size(); ++i) {
            mixFactors[i] /= totalMixFactor;
        }
    }

    if (!mixFactors.empty())
        glUniform1fv(glGetUniformLocation(shader->program, "mix_factors"), mixFactors.size(), mixFactors.data());
    glUniform1i(glGetUniformLocation(shader->program, "numTextures"), texturesBound);

}


/// <summary>
/// Set up matrices for rendering
/// Configure model, view, and projection matrices for the shader.
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="modelMatrix">Model matrix for positioning</param>
/// <param name="orthographicPerspective">Whether to use orthographic perspective</param>
void LightHouse::SetupMatrices(
    Shader* shader,
    const glm::mat4& modelMatrix,
    bool orthographicPerspective)
{
    float WIDTH = static_cast<float>(windowWidth);
    float HEIGHT = static_cast<float>(windowHeight);

    glm::mat4 projectionMatrix = orthographicPerspective ? glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT) : GetSceneCamera()->GetProjectionMatrix();
    glm::mat4 viewMatrix = orthographicPerspective ? glm::mat4(1.0f) : GetSceneCamera()->GetViewMatrix();
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3f(glGetUniformLocation(shader->program, "eye_position"), eyePosition.x, eyePosition.y, eyePosition.z);
    glUniform1f(glGetUniformLocation(shader->program, "time"), Engine::GetElapsedTime());
}


/// <summary>
/// Set up slider properties for rendering
/// Configure shader uniforms specific to sliders.
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="color">Color of the slider</param>
void LightHouse::SetupSlider(Shader* shader, const glm::vec3& color)
{
    glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "u_color"), 1, glm::value_ptr(color));
    glUniform2f(glGetUniformLocation(shader->GetProgramID(), "u_resolution"), static_cast<float>(windowWidth), static_cast<float>(windowHeight));

}


/// <summary>
/// Set up lighting for rendering
/// Configure shader uniforms related to lighting properties. 
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="color">Color of the object being lit</param>
void LightHouse::SetupLighting(Shader* shader, const glm::vec3& color)
{
    glUniform3fv(glGetUniformLocation(shader->program, "objectColor"), 1, glm::value_ptr(color));

    glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 15, glm::value_ptr(point_light_pos[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "light_direction"), 15, glm::value_ptr(point_light_dir[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "light_color"), 15, glm::value_ptr(point_light_color[0]));

    glUniform1ui(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform3fv(glGetUniformLocation(shader->program, "material_ke"), 1, glm::value_ptr(materialKe));
    glUniform1f(glGetUniformLocation(shader->program, "material_ka"), materialKa);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
    
    glUniform1f(glGetUniformLocation(shader->program, "angle"), angleCutOff);
}


void LightHouse::OnKeyPress(int key, int mods)
{
    if (sliderManager->updateSliderSizes(key))
    {
        sliderManager->updateSliderValues();

        if (sliderManager->isRgbChanged()) {
            sliderManager->updateHSVSliders();
        } else if (sliderManager->isHsvChanged()) {
            sliderManager->updateRGBSliders();
        }

        sliderManager->setColor();
        sliderManager->printDebugInfo();
    }
}

void LightHouse::OnInputUpdate(float deltaTime, int mods) {}
void LightHouse::OnKeyRelease(int key, int mods) {}
void LightHouse::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void LightHouse::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void LightHouse::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void LightHouse::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void LightHouse::OnWindowResize(int width, int height) {}
