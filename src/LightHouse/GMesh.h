#pragma once

#ifndef GMESH_H
#define GMESH_H

#include "Creator.h"


class GMesh : public Meshes {
public:
    void Load(
        const std::string& directory,
        const std::string& filename,
        const std::string& name,
        MeshMap& mapMeshes) override;
};

class GMeshCreator : public MeshCreator {
public:
    GMesh* LoadMesh() const override {
        return new GMesh();
    }
};

#endif // GMESH_H
