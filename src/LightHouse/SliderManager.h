#pragma once

#ifndef SLIDER_MANAGER_H
#define SLIDER_MANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>


// Constants for slider and color properties
constexpr float SLIDER_SIZE = 200;
constexpr float RGB_MAX = 255;

constexpr int   HUE = 360, VALUE = 100, SATURATION = 100;
constexpr float HUE_CONE = 360, HUE_DEGREE = 60;
constexpr float VALUE_PERCENT = 100, SATURATION_PERCENT = 100;


struct Slider {
    glm::vec2 position; // Position of the slider
    glm::vec2 size;     // Size of the slider
    glm::vec3 color;    // Color of the interactive component
    float value;        // Current value of the slider

    Slider(const glm::vec2& pos, const glm::vec2& sz, const glm::vec3& col, float val)
        : position(pos), size(sz), color(col), value(val) {}
};


struct hsv_t {
    int hue;        // H [0-360]
    int saturation; // S [0-100]
    int value;      // V [0-100]
};


struct rgb_t {
    int red;        // R [0-255]
    int green;      // G [0-255]
    int blue;       // B [0-255]
};


/// <summary>
/// MANAGES SLIDERS AND COLOR CONVERSION
/// </summary>
class SliderManager {
public:
    SliderManager();
    ~SliderManager();

    bool updateSliderSizes(int key);
    void updateSliderValues();

    void updateRGBSliders();
    void updateHSVSliders();

    void setColor();
    void printDebugInfo();

    const glm::vec3& getLighthouseColor() const { return lighthouseColor; }
    const std::vector<Slider>& getSliders() const { return sliders; }
    bool isRgbChanged() const { return rgbChanged; }
    bool isHsvChanged() const { return hsvChanged; }

private:
    void initializeRgbSliders();
    void initializeHsvSliders();
    void initializeBackgroundSliders();

    // Key mappings for slider control
    const std::unordered_map<int, std::pair<int, float>> keyMappings = {
        {GLFW_KEY_1, {0, 10.0f}}, {GLFW_KEY_2, {0, -10.0f}},    // RED
        {GLFW_KEY_3, {1, 10.0f}}, {GLFW_KEY_4, {1, -10.0f}},    // GREEN
        {GLFW_KEY_5, {2, 10.0f}}, {GLFW_KEY_6, {2, -10.0f}},    // BLUE
        {GLFW_KEY_F, {3, 10.0f}}, {GLFW_KEY_G, {3, -10.0f}},    // HUE
        {GLFW_KEY_H, {4, 10.0f}}, {GLFW_KEY_J, {4, -10.0f}},    // SATURATION
        {GLFW_KEY_K, {5, 10.0f}}, {GLFW_KEY_L, {5, -10.0f}}     // VALUE
    };

    hsv_t hsv;                      // Current HSV values
    rgb_t rgb;                      // Current RGB values
    bool hsvChanged;                // Flag for HSV change
    bool rgbChanged;                // Flag for RGB change
    glm::vec3 lighthouseColor;      // Color of the lighthouse
    std::vector<Slider> sliders;    // Collection of sliders
};

#endif // SLIDER_MANAGER_H
