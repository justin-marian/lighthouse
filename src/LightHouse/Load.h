#pragma once

#ifndef LOAD_H
#define LOAD_H

#include "components/simple_scene.h"

#include <GL/glew.h>
#include <string>
#include <unordered_map>


using Texture2DMap = std::unordered_map<std::string, Texture2D*>;
using ShaderMap = std::unordered_map<std::string, Shader*>;
using MeshMap = std::unordered_map<std::string, Mesh*>;


class Textures {
public:
    virtual ~Textures() {}
    virtual void Load(
        const std::string& path,
        const std::string& name,
        GLint wrappingMode,
        Texture2DMap& mapTextures) = 0;
};


class Meshes {
public:
    virtual ~Meshes() {}
    virtual void Load(
        const std::string& directory,
        const std::string& filename,
        const std::string& name,
        MeshMap& mapMeshes) = 0;
};


class Shaders {
public:
    virtual ~Shaders() {}
    virtual void Load(
        const std::string& vertPath,
        const std::string& fragPath,
        const std::string& name,
        ShaderMap& mapShaders) = 0;
};

#endif // LOAD_H
