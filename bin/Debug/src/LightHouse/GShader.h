#pragma once

#ifndef GSHADER_H
#define GSHADER_H

#include "Creator.h"


class GShader : public Shaders {
public:
    void Load(
        const std::string& vertPath,
        const std::string& fragPath,
        const std::string& name,
        ShaderMap& mapShaders) override;
};

class GShaderCreator : public ShaderCreator {
public:
    GShader* LoadShader() const override {
        return new GShader();
    }
};

#endif // GSHADER_H