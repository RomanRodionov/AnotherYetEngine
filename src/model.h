#pragma once

#include "common.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

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
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<Texture2D> textures;
    Mesh(
        std::vector<Vertex>& vertices_, std::vector<uint>& indices_, std::vector<Texture2D>& textures_) : 
            vertices(std::move(vertices_)), indices(std::move(indices_)), textures(std::move(textures_)) 
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
        directory = std::string(path).substr(0, std::string(path).find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode *node, const aiScene *scene)
    {
        for (uint i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (uint i = 0; i < node->mNumChildren; ++i)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
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
                loadMatTextures(textures, material, it->first, it->second);
            }
        }

        return Mesh(vertices, indices, textures);
    }
    void loadMatTextures(std::vector<Texture2D>& textures, aiMaterial *mat, aiTextureType ai_type, TexType tex_type)
    {
        for(uint i = 0; i < mat->GetTextureCount(ai_type); ++i)
        {
            aiString ai_path;
            mat->GetTexture(ai_type, i, &ai_path);
            std::string path = directory + "/" + std::string(ai_path.C_Str());
            size_t idx = 0;
            while (true) {
                idx = path.find("\\", idx);
                if (idx == std::string::npos) break;
                path.replace(idx, 1, "/");
                idx += 1;
            }
            textures.emplace_back(path.c_str(), tex_type);
            std::cout << "Texture has been loaded: " << path << std::endl;
        }
    }
public:
    Model(const char *path)
    {
        load(path);
    }
    void draw(Shader& shader);
};