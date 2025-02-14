#include "Load.h"
#include "GMesh.h"
#include "GShader.h"
#include "GTexture.h"


void GMesh::Load(
    const std::string& directory,
    const std::string& filename,
    const std::string& name,
    MeshMap& mapMeshes)
{
    Mesh* internalMesh = new Mesh(name);
    internalMesh->LoadMesh(directory, filename);
    mapMeshes[internalMesh->GetMeshID()] = internalMesh;
}


 void GShader::Load(
     const std::string& vertPath,
     const std::string& fragPath,
     const std::string& name,
     ShaderMap& mapShaders)
 {
     Shader* internalShader = new Shader(name);
     internalShader->AddShader(vertPath, GL_VERTEX_SHADER);
     internalShader->AddShader(fragPath, GL_FRAGMENT_SHADER);
     internalShader->CreateAndLink();
     mapShaders[internalShader->GetName()] = internalShader;
 }


void GTexture::Load(
    const std::string& path,
    const std::string& name,
    GLint glMode,
    Texture2DMap& mapTextures)
{
    Texture2D* internalTexture = new Texture2D();
    internalTexture->Load2D(path.c_str(), glMode);
    mapTextures[name] = internalTexture;
}
