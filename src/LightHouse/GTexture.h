#pragma once

#ifndef GTEXTURE_H
#define GTEXTURE_H

#include "Creator.h"


class GTexture : public Textures {
public:
    void Load(
        const std::string& path,
        const std::string& name,
        GLint glMode,
        Texture2DMap& mapTextures) override;
};

class GTextureCreator : public TextureCreator {
public:
    GTexture* LoadTexture() const override {
        return new GTexture();
    }
};

#endif // GTEXTURE_H
