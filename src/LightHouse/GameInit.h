#pragma once

#ifndef GAMEINIT_H
#define GAMEINIT_H

#include "components/simple_scene.h"
#include "components/transform.h"

#include <string>
#include <unordered_map>


class GameInit : public gfxc::SimpleScene {
    class LightHouse;

public:
    GameInit(
        std::unordered_map<std::string, Mesh*>& initMeshes,
        std::unordered_map<std::string, Shader*>& initShaders,
        std::unordered_map<std::string, Texture2D*>& initTextures
    ) : meshes(initMeshes), shaders(initShaders), textures(initTextures) {}

    void LoadResources();
    Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
    Mesh* CreateGridMesh(const int gridX, const int gridZ, const float gridSizeX, const float gridSizeZ);

private:
    void LoadAllTextures();
    void LoadAllMeshes();
    void LoadAllShaders();

    std::unordered_map<std::string, Mesh*>& meshes;
    std::unordered_map<std::string, Shader*>& shaders;
    std::unordered_map<std::string, Texture2D*>& textures;
};

#endif // GAMEINIT_H
