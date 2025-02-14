#include "GameInit.h"
#include "GMesh.h"
#include "GShader.h"
#include "GTexture.h"

#include <fstream>
#include <cstdlib>
#include <vector>


/// <summary>
/// Load all resources (textures, meshes, shaders) required for the game.
/// </summary>
void GameInit::LoadResources()
{
    LoadAllTextures();
    LoadAllMeshes();
    LoadAllShaders();
}


/// <summary>
/// Load all meshes from specified directories.
/// </summary>
void GameInit::LoadAllMeshes()
{
    // Define directories for different object models
    const std::string sourceObjsDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT, "LightHouse", "objs");
    const std::string sourcePrimitives = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives");
    const std::string sourceBamboo = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo");

    // Subdirectories
    const std::string sourceLightHouse = PATH_JOIN(sourceObjsDir, "lighthouse");
    const std::string sourceBoats = PATH_JOIN(sourceObjsDir, "boats");
    const std::string sourceMoon = PATH_JOIN(sourceObjsDir, "moon");
    const std::string sourceLake = PATH_JOIN(sourceObjsDir, "lake");

    // The meshCreator object creates GMesh objects from files
    GMeshCreator* meshCreator = new GMeshCreator();
    GMesh* mesh = meshCreator->LoadMesh();

    // Load meshes for various objects like lighthouse, boats, moon, lake, bamboo
    mesh->Load(sourceLightHouse, "lighthouse.obj", "lighthouse", meshes);
    mesh->Load(sourceBoats, "wake_boat.glb", "wake_boat", meshes);
    mesh->Load(sourceMoon, "sphere.obj", "sphere", meshes);
    mesh->Load(sourceLake, "gridMesh.obj", "lake", meshes);
    mesh->Load(sourceBamboo, "bamboo.obj", "bamboo", meshes);
}


/// <summary>
/// Load all shader programs.
/// </summary>
void GameInit::LoadAllShaders()
{
    // Define directories for shader files
    const std::string sourceShadersDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT, "LightHouse", "shaders");
    const std::string sourceSliderVERTEXDir = PATH_JOIN(sourceShadersDir, "Sliders", "vertex");
    const std::string sourceSliderFRAGMENTDir = PATH_JOIN(sourceShadersDir, "Sliders", "fragment");

    // The shaderCreator object creates GShader objects from shader files
    GShaderCreator* shaderCreator = new GShaderCreator();
    GShader* shader = shaderCreator->LoadShader();

    // Load shader programs for different rendering effects
    /// ALL OBJECTS
    shader->Load(PATH_JOIN(sourceShadersDir, "VertexShader.glsl"),
        PATH_JOIN(sourceShadersDir, "FragmentShader.glsl"), "Scene", shaders);
    /// LAKE + MOUNTAINS
    shader->Load(PATH_JOIN(sourceShadersDir, "V_Moutain.glsl"),
        PATH_JOIN(sourceShadersDir, "F_Mountain.glsl"), "Lake", shaders);
    /// MOON + LIGHTHOUSE (TOP PART)
    shader->Load(PATH_JOIN(sourceShadersDir, "V_LightHouse.glsl"),
        PATH_JOIN(sourceShadersDir, "F_LightHouse.glsl"), "LightHouse", shaders);
    /// RGB
    shader->Load(PATH_JOIN(sourceSliderVERTEXDir, "V_RGB.glsl"),
        PATH_JOIN(sourceSliderFRAGMENTDir, "F_RGB.glsl"), "RGB", shaders);
    /// HVS
    shader->Load(PATH_JOIN(sourceSliderVERTEXDir, "V_HSV.glsl"),
        PATH_JOIN(sourceSliderFRAGMENTDir, "F_Hue.glsl"), "HUE", shaders);
    shader->Load(PATH_JOIN(sourceSliderVERTEXDir, "V_HSV.glsl"),
        PATH_JOIN(sourceSliderFRAGMENTDir, "F_Saturation.glsl"), "SAT", shaders);
    shader->Load(PATH_JOIN(sourceSliderVERTEXDir, "V_HSV.glsl"),
        PATH_JOIN(sourceSliderFRAGMENTDir, "F_Value.glsl"), "VAL", shaders);
}


/// <summary>
/// Load all textures from specified directories.
/// </summary>
void GameInit::LoadAllTextures()
{
    // Define directories for different texture categories
    const std::string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT, "LightHouse", "textures");
    const std::string sourceBamboo = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo");
    const std::string boatsTextureDir = PATH_JOIN(sourceTextureDir, "boats");
    const std::string groundTextureDir = PATH_JOIN(sourceTextureDir, "ground");
    const std::string moonskyTextureDir = PATH_JOIN(sourceTextureDir, "moon-sky");
    const std::string lighthouseTextureDir = PATH_JOIN(sourceTextureDir, "light-house");

    // The textureCreator object creates GTexture objects from image files
    GTextureCreator* textureCreator = new GTextureCreator();
    GTexture* texture = textureCreator->LoadTexture();
    
    /// Load textures for various objects like lighthouse, boats, ground, moon, bamboo

    // Light-House Textures
    std::string lightHouseWhiteTexturePath = PATH_JOIN(lighthouseTextureDir, "light_house_white.jpg"); // top_house.jpg
    std::string lightHouseRedTexturePath = PATH_JOIN(lighthouseTextureDir, "LIGHT.jpg"); // mid_house.jpg
    std::string baseHouseTexturePath = PATH_JOIN(lighthouseTextureDir, "base_house.jpg");
    texture->Load(lightHouseWhiteTexturePath.c_str(), "top-house", GL_REPEAT, textures);
    texture->Load(lightHouseRedTexturePath.c_str(), "mid-house", GL_REPEAT, textures);
    texture->Load(baseHouseTexturePath.c_str(), "base-house", GL_REPEAT, textures);

    // Boat Textures
    std::string darkIronTexturePath = PATH_JOIN(boatsTextureDir, "iron_dark.jpg");
    std::string rustyIronTexturePath = PATH_JOIN(boatsTextureDir, "iron_rust.jpg");
    std::string wood1TexturePath = PATH_JOIN(boatsTextureDir, "wood1.jpg");
    std::string wood2TexturePath = PATH_JOIN(boatsTextureDir, "wood2.jpg");
    std::string wood3TexturePath = PATH_JOIN(boatsTextureDir, "wood3.jpg");
    texture->Load(darkIronTexturePath.c_str(), "iron_dark", GL_MIRROR_CLAMP_TO_EDGE, textures);
    texture->Load(rustyIronTexturePath.c_str(), "iron_rust", GL_MIRROR_CLAMP_TO_EDGE, textures);
    texture->Load(wood1TexturePath.c_str(), "wood1", GL_MIRRORED_REPEAT, textures);
    texture->Load(wood2TexturePath.c_str(), "wood2", GL_MIRRORED_REPEAT, textures);
    texture->Load(wood3TexturePath.c_str(), "wood3", GL_MIRRORED_REPEAT, textures);

    // Basic Ground Textures
    std::string rockTexturePath = PATH_JOIN(groundTextureDir, "ground.jpg");
    std::string groundHMapPath = PATH_JOIN(groundTextureDir, "groundHMap.jpg");
    texture->Load(rockTexturePath.c_str(), "ground", GL_REPEAT, textures);
    texture->Load(groundHMapPath.c_str(), "groundHMap", GL_REPEAT, textures);

    // Water Ground Textures
    std::string waterTexturePath = PATH_JOIN(groundTextureDir, "water.jpg");
    std::string waterUVPath = PATH_JOIN(groundTextureDir, "waterUV.jpg");
    texture->Load(waterTexturePath.c_str(), "water", GL_REPEAT, textures);
    texture->Load(waterUVPath.c_str(), "waterUV", GL_REPEAT, textures);

    // Lava Ground Textures
    std::string lavaTexturePath = PATH_JOIN(groundTextureDir, "lava.jpg");
    std::string lavaUVPath = PATH_JOIN(groundTextureDir, "lavaUV.jpg");
    std::string lavaOccPath = PATH_JOIN(groundTextureDir, "lavaOcc.jpg");
    texture->Load(lavaTexturePath.c_str(), "lava", GL_REPEAT, textures);
    texture->Load(lavaUVPath.c_str(), "lavaUV", GL_REPEAT, textures);
    texture->Load(lavaOccPath.c_str(), "lavaOcc", GL_REPEAT, textures);

    // Moon and Sky Texturess
    std::string moonTexturePath = PATH_JOIN(moonskyTextureDir, "moon.jpg");
    std::string moonHMapPath = PATH_JOIN(moonskyTextureDir, "moonHMap.jpeg");
    texture->Load(moonTexturePath.c_str(), "moon", GL_REPEAT, textures);
    texture->Load(moonHMapPath.c_str(), "moonHMap", GL_REPEAT, textures);

    // Bamboo Texture
    std::string bambooTexturePath = PATH_JOIN(sourceBamboo, "bamboo.png");
    texture->Load(bambooTexturePath.c_str(), "bamboo", GL_REPEAT, textures);
}


/// <summary>
/// 
/// </summary>
/// <param name="name"></param>
/// <param name="vertices"></param>
/// <param name="indices"></param>
/// <returns></returns>
Mesh* GameInit::CreateMesh(
    const char* name,
    const std::vector<VertexFormat>& vertices,
    const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
        (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
        (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
        (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


///         ! WARNING ! 
///         IT IS UNUSED
///     IT WAS ONLY USED ONCE 
///   TO CREATE THE GRID OBJECT AT
/// \\Users\\USERNAME\\Desktop\\ PATH

/// <summary>
/// 
/// </summary>
/// <param name="gridX"></param>
/// <param name="gridZ"></param>
/// <param name="gridSizeX"></param>
/// <param name="gridSizeZ"></param>
/// <returns></returns>
Mesh* GameInit::CreateGridMesh(
    const int gridX,
    const int gridZ,
    const float gridSizeX,
    const float gridSizeZ)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Calculate half sizes to center the grid at the origin
    float halfSizeX = gridSizeX * 0.5f;
    float halfSizeZ = gridSizeZ * 0.5f;

    // Calculate the step between each vertex
    float dx = gridSizeX / static_cast<float>(gridX - 1);
    float dz = gridSizeZ / static_cast<float>(gridZ - 1);

    // Calculate the texture step
    float dTexX = 1.0f / static_cast<float>(gridX - 1);
    float dTexZ = 1.0f / static_cast<float>(gridZ - 1);

    // Create vertices
    for (int z = 0; z < gridZ; ++z)
    {
        for (int x = 0; x < gridX; ++x)
        {
            // Position
            // Invert Z to match typical OpenGL coordinates
            float posX = -halfSizeX + x * dx;
            float posZ = halfSizeZ - z * dz;
            glm::vec3 position(posX, 0.0f, posZ);

            // Normal is up because it's a flat plane
            glm::vec3 normal(0.0f, 1.0f, 0.0f);

            // Texture coordinates
            glm::vec2 text_coord(dTexX * x, dTexZ * z);

            // Color (default white)
            glm::vec3 color(1.0f);

            // Add vertex to the list
            vertices.push_back(VertexFormat(position, color, normal, text_coord));
        }
    }

    // Create indices for 2 triangles per quad of the grid
    for (int z = 0; z < gridZ - 1; ++z)
    {
        for (int x = 0; x < gridX - 1; ++x)
        {
            unsigned int topLeft = (z * gridX) + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = ((z + 1) * gridX) + x;
            unsigned int bottomRight = bottomLeft + 1;

            // Triangle 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triangle 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    std::string desktopPath = "C:\\Users\\";
    desktopPath += getenv("USERNAME");
    desktopPath += "\\Desktop\\gridMesh.obj";

    std::ofstream objFile(desktopPath);

    // Write vertices
    for (const auto& vertex : vertices)
    {
        objFile << "v " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << "\n";
        objFile << "vn " << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << "\n";
        objFile << "vt " << vertex.text_coord.x << " " << vertex.text_coord.y << "\n";
    }

    // Write faces
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        objFile << "f "
            << indices[i] + 1 << "/" << indices[i] + 1 << "/" << indices[i] + 1 << " "
            << indices[i + 1] + 1 << "/" << indices[i + 1] + 1 << "/" << indices[i + 1] + 1 << " "
            << indices[i + 2] + 1 << "/" << indices[i + 2] + 1 << "/" << indices[i + 2] + 1 << "\n";
    }

    objFile.close();

    // Flat plane similar with plane50 (with 200 vertices)
    return CreateMesh("plane200", vertices, indices);
}
