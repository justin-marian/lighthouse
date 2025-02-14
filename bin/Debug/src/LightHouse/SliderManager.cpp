#include "SliderManager.h"

#include <iostream>


SliderManager::SliderManager()
{
    hsv.hue = hsv.saturation = hsv.value = 0;
    rgbChanged = hsvChanged = false;
    rgb.red = rgb.green = rgb.blue = 0;
    lighthouseColor = glm::vec3(0.5f, 0.5f, 0.5f);

    initializeRgbSliders();
    initializeHsvSliders();
    initializeBackgroundSliders();
}


SliderManager::~SliderManager() {}

/// https://cs.stackexchange.com/questions/64549/convert-hsv-to-rgb-colors
/// <summary>
/// Convert RGB to HSV colors
/// </summary>
/// <param name="rgb">RGB color to be converted.</param>
/// <param name="hsv">Resulting HSV color.</param>
static inline void rgb2hsv(const rgb_t& rgb, hsv_t& hsv)
{
    // Normalize RGB values
    float red = rgb.red / RGB_MAX;
    float green = rgb.green / RGB_MAX;
    float blue = rgb.blue / RGB_MAX;

    // Calculate maximum and minimum RGB values
    float cMax = std::max({ red, green, blue });
    float cMin = std::min({ red, green, blue });
    float delta = cMax - cMin;

    // Hue Calculation - determines the color type
    if (delta == 0) {
        hsv.hue = 0; // Achromatic
    }
    else {
        if (cMax == red) {
            hsv.hue = HUE_DEGREE * fmod(((green - blue) / delta), 6);
        }
        else if (cMax == green) {
            hsv.hue = HUE_DEGREE * (((blue - red) / delta) + 2);
        }
        else { // cMax == blue
            hsv.hue = HUE_DEGREE * (((red - green) / delta) + 4);
        }
        if (hsv.hue < 0) {
            hsv.hue += HUE_CONE;
        }
    }

    // Saturation Calculation - determines the vibrance of the color
    hsv.saturation = (cMax == 0) ? 0 : static_cast<int>((delta / cMax) * SATURATION_PERCENT);
    // Value Calculation - determines the brightness of the color
    hsv.value = static_cast<int>(cMax * VALUE_PERCENT);
}


/// https://math.hws.edu/graphicsbook/demos/c2/rgb-hsv.html
/// <summary>
/// Convert HSV to RGB colors
/// </summary>
/// <param name="hsv">HSV color to be converted.</param>
/// <param name="rgb">Resulting RGB color.</param>
static inline void hsv2rgb(const hsv_t& hsv, rgb_t& rgb)
{
    // Divide hue into 6 sectors for conversion
    float h = static_cast<float>(hsv.hue) / HUE_DEGREE;
    float s = static_cast<float>(hsv.saturation) / SATURATION_PERCENT;
    float v = static_cast<float>(hsv.value) / VALUE_PERCENT;

    // Calculate RGB values based on HSV model
    int i = static_cast<int>(floor(h));
    float f = h - i; // factorial part of h
    float p = v * (1 - s);
    float q = v * (1 - s * f);
    float t = v * (1 - s * (1 - f));

    // Assigning temporary RGB values based on sector
    float rTemp, gTemp, bTemp;
    switch (i) {
        case 0: rTemp = v; gTemp = t; bTemp = p; break;
        case 1: rTemp = q; gTemp = v; bTemp = p; break;
        case 2: rTemp = p; gTemp = v; bTemp = t; break;
        case 3: rTemp = p; gTemp = q; bTemp = v; break;
        case 4: rTemp = t; gTemp = p; bTemp = v; break;
        default: rTemp = v; gTemp = p; bTemp = q; break;
    }

    // Convert temporary RGB values to final values based on the maximum RGB value
    rgb.red = static_cast<int>(rTemp * RGB_MAX);
    rgb.green = static_cast<int>(gTemp * RGB_MAX);
    rgb.blue = static_cast<int>(bTemp * RGB_MAX);
}


/// <summary>
/// Initialize RGB sliders
/// </summary>
void SliderManager::initializeRgbSliders()
{
    // Set positions and properties for RGB sliders
    const float sliderWidth = 200.0f, sliderLength = 50.0f;
    const glm::vec2 firstSliderPosition(20, 20);
    const float verticalSpacing = 10.0f;

    // Initialize and position RGB sliders
    glm::vec2 position = firstSliderPosition;
    sliders.push_back(Slider(position, glm::vec2(sliderWidth, sliderLength), glm::vec3(1.f, 0.f, 0.f), 0.f));

    position.y += sliderLength + verticalSpacing;
    sliders.push_back(Slider(position, glm::vec2(sliderWidth, sliderLength), glm::vec3(0.f, 1.f, 0.f), 0.f));

    position.y += sliderLength + verticalSpacing;
    sliders.push_back(Slider(position, glm::vec2(sliderWidth, sliderLength), glm::vec3(0.f, 0.f, 1.f), 0.f));
}


/// <summary>
/// Initialize HSV sliders
/// </summary>
void SliderManager::initializeHsvSliders()
{
    // Set positions and properties for HSV sliders
    const float sliderWidth = 200.0f, sliderLength = 50.0f, horizontalSpacing = 10.0f;
    float hsvXPosition = sliders[0].position.x + sliderWidth + horizontalSpacing;

    // Initialize and position HSV sliders
    glm::vec2 position(hsvXPosition, sliders[0].position.y);
    sliders.push_back(Slider(position, glm::vec2(sliderWidth, sliderLength), glm::vec3(0.2f, 0.2f, 0.2f), 0.f));

    position.y = sliders[1].position.y;
    sliders.push_back(Slider(position, glm::vec2(sliderWidth, sliderLength), glm::vec3(0.2f, 0.2f, 0.2f), 0.f));

    position.y = sliders[2].position.y;
    sliders.push_back(Slider(position, glm::vec2(sliderWidth, sliderLength), glm::vec3(0.25f, 0.25f, 0.25f), 0.f));
}


/// <summary>
/// Initialize background sliders for RGB and HSV sliders
/// </summary>
void SliderManager::initializeBackgroundSliders()
{
    // Create background sliders for aesthetics and UI clarity
    for (int i = 0; i < 6; ++i)
    {
        glm::vec3 color(0.2f, 0.2f, 0.2f);
        if (i >= 3) color = glm::vec3(0.25f, 0.25f, 0.25f); // Differentiate HSV slider background
        sliders.push_back(Slider(sliders[i].position, sliders[i].size, color, 0.0f));
    }
}


/// <summary>
///             Update Slider Sizes Based on User Input
/// Updates the sizes of the sliders based on the user's key input.
/// </summary>
/// <param name="key">Key code representing the user's input.</param>
/// <returns>True if the size of any slider changed, otherwise false.</returns>
bool SliderManager::updateSliderSizes(int key)
{
    bool sizeChanged = false;
    rgbChanged = hsvChanged = false;

    // Check if a slider is an RGB slider
    auto isRGBSlider = [](int idxSlider) -> bool { return idxSlider < 3; };

    // Update flags when a slider size is changed
    auto usedSliderIndex = [this, &sizeChanged, isRGBSlider](int idxSlider) {
        sizeChanged = true;
        if (isRGBSlider(idxSlider)) {
            rgbChanged = true;
        } else {
            hsvChanged = true;
        }
    };

    // Change the size of a slider and update flags accordingly
    auto sizeSlider = [this, &usedSliderIndex](int idxSlider, float delta) {
        float oldSize = sliders[idxSlider].size.x;
        sliders[idxSlider].size.x = glm::clamp(sliders[idxSlider].size.x + delta, 0.0f, SLIDER_SIZE);
        if (oldSize != sliders[idxSlider].size.x) usedSliderIndex(idxSlider);
    };

    // Check if the key corresponds to a slider and adjust its size
    auto it = keyMappings.find(key);
    if (it != keyMappings.end())
    {
        int idxSlider = it->second.first;
        float delta = it->second.second;
        sizeSlider(idxSlider, delta);
    }

    return sizeChanged;
}


/// <summary>
/// Update slider values based on their sizes
/// </summary>
void SliderManager::updateSliderValues()
{
    // Calculate and update the value of each slider based on its size
    for (int i = 0; i < sliders.size(); ++i)
    {
        sliders[i].value = sliders[i].size.x / SLIDER_SIZE;
    }
}


/// <summary>
/// Update RGB sliders based on HSV values
/// </summary>
void SliderManager::updateRGBSliders()
{
    // Convert HSV to RGB and update RGB sliders
    hsv_t hsv{ sliders[3].value * HUE,
               sliders[4].value * SATURATION,
               sliders[5].value * VALUE };
    hsv2rgb(hsv, rgb);

    sliders[0].size.x = static_cast<float>(rgb.red) / RGB_MAX * SLIDER_SIZE;
    sliders[1].size.x = static_cast<float>(rgb.green) / RGB_MAX * SLIDER_SIZE;
    sliders[2].size.x = static_cast<float>(rgb.blue) / RGB_MAX * SLIDER_SIZE;
}


/// <summary>
/// Update HSV sliders based on RGB values
/// </summary>
void SliderManager::updateHSVSliders()
{
    // Convert RGB to HSV and update HSV sliders
    rgb_t rgb{ static_cast<int>(sliders[0].value * RGB_MAX),
                static_cast<int>(sliders[1].value * RGB_MAX),
                static_cast<int>(sliders[2].value * RGB_MAX) };
    hsv_t hsv;
    rgb2hsv(rgb, hsv);


    sliders[3].size.x = static_cast<float>(hsv.hue) / HUE_CONE * SLIDER_SIZE;
    sliders[4].size.x = static_cast<float>(hsv.saturation) / SATURATION_PERCENT * SLIDER_SIZE;
    sliders[5].size.x = static_cast<float>(hsv.value) / VALUE_PERCENT * SLIDER_SIZE;

    // Update HSV sliders based on the converted values
    sliders[3].value = static_cast<float>(hsv.hue) / HUE_CONE;
    sliders[4].value = static_cast<float>(hsv.saturation) / SATURATION_PERCENT;
    sliders[5].value = static_cast<float>(hsv.value) / VALUE_PERCENT;
}


/// <summary>
/// Print current RGB and HSV values for debugging
/// </summary>
void SliderManager::printDebugInfo()
{
    // Output the current values of RGB and HSV sliders to the console

    // Print RGB values
    std::cout << "RGB (normalized): ["
        << sliders[0].value << ", "
        << sliders[1].value << ", "
        << sliders[2].value << "]" << std::endl;

    // Print HSV values
    std::cout << "HSV: ["
        << sliders[3].value * HUE_CONE << " (Hue), "
        << sliders[4].value * SATURATION_PERCENT << "% (Saturation), "
        << sliders[5].value * VALUE_PERCENT << "% (Value)]" << std::endl;
}


/// <summary>
/// Set the color of the lighthouse based on RGB slider values
/// </summary>
void SliderManager::setColor()
{
    // Update lighthouse color based on RGB slider values
    float r = sliders[0].value;
    float g = sliders[1].value;
    float b = sliders[2].value;
    lighthouseColor = glm::vec3(r, g, b);
}
