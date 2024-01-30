#pragma once

#include "common.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include <filesystem>

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh
{
private:
    GLuint vertexArrayID, vertexBufferID, elementBufferID;
    void setup();
public:
    glm::mat4x4 m_transform;
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<Texture2D> textures;
    Mesh(
        std::vector<Vertex>& vertices_, std::vector<uint>& indices_, std::vector<Texture2D>& textures_) : 
            vertices(std::move(vertices_)), indices(std::move(indices_)), textures(std::move(textures_)) 
    {
        setup();
        m_transform = glm::mat4x4(1.f);
    }
    Mesh(
        std::vector<Vertex>& vertices_, std::vector<uint>& indices_, 
        std::vector<Texture2D>& textures_, glm::mat4x4 transform) : 
            vertices(std::move(vertices_)), indices(std::move(indices_)),
            textures(std::move(textures_)), m_transform(transform)
    {
        setup();
    }
    void draw(Shader& shader);
};

const std::map<aiTextureType, TexType> texTypesImage = {
    {aiTextureType_NONE, TexType::NONE},
    {aiTextureType_DIFFUSE, TexType::DIFFUSE},
    {aiTextureType_SPECULAR, TexType::SPECULAR},
    {aiTextureType_AMBIENT, TexType::AMBIENT},
    {aiTextureType_EMISSIVE, TexType::EMISSIVE},
    {aiTextureType_HEIGHT, TexType::HEIGHT},
    {aiTextureType_NORMALS, TexType::NORMALS},
    {aiTextureType_SHININESS, TexType::SHININESS},
    {aiTextureType_OPACITY, TexType::OPACITY},
    {aiTextureType_DISPLACEMENT, TexType::DISPLACEMENT},
    {aiTextureType_LIGHTMAP, TexType::DISPLACEMENT}
};

class Model
{
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void load(const char* path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }
        directory = std::filesystem::path(path).parent_path().string();

        
        int upAxis = 2;
        scene->mMetaData->Get<int>("UpAxis", upAxis);
        int upAxisSign = 1;
        scene->mMetaData->Get<int>("UpAxisSign", upAxisSign);
        int frontAxis = 1;
        scene->mMetaData->Get<int>("FrontAxis", frontAxis);
        int frontAxisSign = 1;
        scene->mMetaData->Get<int>("FrontAxisSign", frontAxisSign);
        int coordAxis = 0;
        scene->mMetaData->Get<int>("CoordAxis", coordAxis);
        int coordAxisSign = 1;
        scene->mMetaData->Get<int>("CoordAxisSign", coordAxisSign);
        float scaleFactor = 10.f;
        //scene->mMetaData->Get<float>("UnitScaleFactor", scaleFactor);

        aiVector3D upVec = upAxis == 0 ? aiVector3D(upAxisSign,0,0) : upAxis == 1 ? aiVector3D(0, upAxisSign,0) : aiVector3D(0, 0, upAxisSign);
        aiVector3D forwardVec = frontAxis == 0 ? aiVector3D(frontAxisSign, 0, 0) : frontAxis == 1 ? aiVector3D(0, frontAxisSign, 0) : aiVector3D(0, 0, frontAxisSign);
        aiVector3D rightVec = coordAxis == 0 ? aiVector3D(coordAxisSign, 0, 0) : coordAxis == 1 ? aiVector3D(0, coordAxisSign, 0) : aiVector3D(0, 0, coordAxisSign);
        aiMatrix4x4 transMat(rightVec.x, rightVec.y, rightVec.z, 0.f,
                        upVec.x, upVec.y, upVec.z, 0.f,
                        forwardVec.x, forwardVec.y, forwardVec.z, 0.f,
                        0.f, 0.f, 0.f, 1.f);

        transMat *= aiMatrix4x4(1 / scaleFactor, 0.f, 0.f, 0.f,
                                0.f, 1 / scaleFactor, 0.f, 0.f,
                                0.f, 0.f, 1 / scaleFactor, 0.f,
                                0.f, 0.f, 0.f, 1.f);
        
        scene->mRootNode->mTransformation *= transMat;
        
        processNode(scene->mRootNode, scene, scene->mRootNode->mTransformation);
    }
    void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 transform)
    {
        transform *= node->mTransformation;
        for (uint i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene, transform));
        }

        for (uint i = 0; i < node->mNumChildren; ++i)
        {
            processNode(node->mChildren[i], scene, transform);
        }
    }
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transform)
    {
        std::vector<Vertex> vertices;
        std::vector<uint> indices;
        std::vector<Texture2D> textures;

        for (uint i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex;

            vertex.pos.x = mesh->mVertices[i].x;
            vertex.pos.y = mesh->mVertices[i].y;
            vertex.pos.z = mesh->mVertices[i].z;

            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;

            if (mesh->mTextureCoords[0])
            {
                vertex.uv.x = mesh->mTextureCoords[0][i].x;
                vertex.uv.y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                vertex.uv = glm::vec2(0.f, 0.f);
            }

            vertices.push_back(vertex);
        }

        for (uint i = 0; i < mesh->mNumFaces; ++i)
        {
            auto face = mesh->mFaces[i];
            for (uint j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0)
        {
            auto *material = scene->mMaterials[mesh->mMaterialIndex];
            for (auto it = texTypesImage.begin(); it != texTypesImage.end(); it++)
            {
                loadMatTextures(textures, scene, material, it->first, it->second);
            }
        }

        return Mesh(vertices, indices, textures, aiMatrix4x4ToGlm(transform));
    }
    void loadMatTextures(std::vector<Texture2D>& textures, const aiScene *scene, aiMaterial *mat, aiTextureType ai_type, TexType tex_type)
    {
        for(uint i = 0; i < mat->GetTextureCount(ai_type); ++i)
        {
            aiString ai_path;
            mat->Get(AI_MATKEY_TEXTURE(ai_type, i), ai_path);
            if(auto texture = scene->GetEmbeddedTexture(ai_path.C_Str())) 
            {
                if (texture->mHeight > 0)
                {
                    std::cout << texture->mHeight << " " << texture->mWidth << std::endl;
                    textures.emplace_back(texture, tex_type);
                    std::cout << "Texture(" << int(tex_type) << ") has been loaded: embedded" << std::endl;
                    //std::cout << "Texture(" << int(tex_type) << ") has been loaded: " << texture->mFilename.C_Str() << " (embedded)" << std::endl;
                }
                else
                {
                    std::string path = std::string(texture->mFilename.C_Str());
                    size_t idx = 0;
                    while (true) {
                        idx = path.find(" ", idx);
                        if (idx == std::string::npos) break;
                        path.replace(idx, 1, "_");
                        idx += 1;
                    }
                    std::size_t path_part = path.find_last_of("/\\");
                    path = path.substr(path_part+1);
                    path = directory + "/textures/" + path;
                    
                    textures.emplace_back(path.c_str(), tex_type);
                    std::cout << "Texture(" << int(tex_type) << ") has been loaded: " << path << std::endl;
                }
            }
            else
            {
                std::string path = std::string(ai_path.C_Str());
                size_t idx = 0;
                while (true) {
                    idx = path.find(" ", idx);
                    if (idx == std::string::npos) break;
                    path.replace(idx, 1, "_");
                    idx += 1;
                }
                std::size_t path_part = path.find_last_of("/\\");
                path = path.substr(path_part+1);
                path = directory + "/textures/" + path;
                
                textures.emplace_back(path.c_str(), tex_type);
                std::cout << "Texture(" << int(tex_type) << ") has been loaded: " << path << std::endl;
            }

            

        }
    }
public:
    Model(const char *path)
    {
        load(path);
    }
    void draw(Shader& shader);
};