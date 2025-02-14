#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"

#include "GameInit.h"
#include "SliderManager.h"

#include <random>
#include <string>
#include <unordered_map>


class LightHouse : public gfxc::SimpleScene
{
public:
    LightHouse();
    ~LightHouse();
    void Init() override;

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void SetupSlider(Shader* shader, const glm::vec3& color);
    void SetupLighting(Shader* shader, const glm::vec3& color);
    void SetupMatrices(Shader* shader, const glm::mat4& modelMatrix, bool orthographicPerspective);
    void SetupTextures(Shader* shader, const std::vector<Texture2D*>& textures, std::vector<float>& mixFactors);

    void LightHouse::RenderMoon();
    void LightHouse::RenderLakePlane();


    void RenderTextured(
        Mesh* mesh,
        Shader* shader,
        const glm::mat4& modelMatrix,
        std::vector<Texture2D*> textures = {},
        std::vector<float> mixFactors = {},
        const glm::vec3& color = glm::vec3(0),
        bool ortographic_perspective = false); // DEFAULT PERSPECTIVE

    void RenderBoats(float deltaTimeSeconds);
    void RenderLighthouseObject();
    void SetupLighthouseLighting();
    void UpdateAndRenderUpperLayer();
    void RenderBamboos();
    void RenderSliders();
    void RenderSlider(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

    float elapsedTime;
    int windowWidth, windowHeight;
    glm::ivec2 resolution;

    glm::vec3 lighthousePosition = glm::vec3(0, 1, 0);

    GameInit* gameInit;
    SliderManager* sliderManager;
    std::unordered_map<std::string, Texture2D*> textures;
        
    /// BOATS ///

    float boatRotationAngles[4];
    float boatRotationSpeeds[4];
    float radiusDist[4];
    int   boatRotationDirections[4];
    std::vector<glm::vec3> boatInitialColors;

    /// LIGHTS ///

    /// 4 BOATS + 2 LIGHTHOUSE + 1 MOON + 8 BASE LIGHTHOUSE
    glm::vec3 point_light_pos[15];
    glm::vec3 point_light_color[15];
    glm::vec3 point_light_dir[15];

    unsigned int materialShininess;
    float materialKd;
    float materialKs;
    float materialKa;
    glm::vec3 materialKe;

    float angleCutOff;
};
