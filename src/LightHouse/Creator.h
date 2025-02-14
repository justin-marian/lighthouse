#pragma once

#ifndef CREATOR_H
#define CREATOR_H

#include "components/simple_scene.h"
#include "Load.h"

#include <GL/glew.h>

#include <string>
#include <unordered_map>


class GTexture;
class GMesh;
class GShader;


class TextureCreator {
public:
    virtual ~TextureCreator() {}
    virtual GTexture* LoadTexture() const = 0;
};


class MeshCreator {
public:
    virtual ~MeshCreator() {}
    virtual GMesh* LoadMesh() const = 0;
};


class ShaderCreator {
public:
    virtual ~ShaderCreator() {}
    virtual GShader* LoadShader() const = 0;
};

#endif // CREATOR_H
