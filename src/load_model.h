#pragma once
#include "common.h"
#include <cstring>
#include <map>
#include "normal_maps.h"

struct UniqueVertex
{
    uint v, uv, n;
    UniqueVertex(uint v_idx, uint uv_idx, uint n_idx) : v(v_idx), uv(uv_idx), n(n_idx) {}
    bool operator==(const UniqueVertex &vert) const {
        if (v == vert.v && uv == vert.uv && n == vert.n) 
            return true;
        return false;
    }
    bool operator<(const UniqueVertex &vert) const {
        if (v < vert.v) return true;
        else if (v == vert.v)
        {
            if (uv < vert.uv) return true;
            else if (uv == vert.uv)
            {
                if (n < vert.n) return true;
                return false;
            }
            return false;
        }
        return false;
    }
};

bool loadOBJ
(
    const std::string &path,
    std::vector<uint> &indices,
    std::vector<glm::vec3> &vertices,
    std::vector<glm::vec2> &uvs,
    std::vector<glm::vec3> &normals
)
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    FILE * file = fopen(path.c_str(), "r");
    if (file == NULL)
    {
        std::cout << "I can't read this file, SEMPAI (((:\n" << path << std::endl;
        return false;
    }   
    while (true)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string v1, v2, v3;
            unsigned int vertexInd[3], uvInd[3], normalInd[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                    &vertexInd[0], &uvInd[0], &normalInd[0],
                    &vertexInd[1], &uvInd[1], &normalInd[1],
                    &vertexInd[2], &uvInd[2], &normalInd[2]);
            if (matches != 9)
            {
                std::cout << "I can't handle this file, SEMPAI !!!\n" << path << std::endl;
                return false;
            }
            for (int i = 0; i < 3; ++i)
            {
                vertexIndices.push_back(vertexInd[i]);
                uvIndices.push_back(uvInd[i]);
                normalIndices.push_back(normalInd[i]);
            }

        }
    } 
    std::map<UniqueVertex, uint> verticesMap;
    uint index = 0;
    for (uint i = 0; i < vertexIndices.size(); ++i)
    {
        UniqueVertex unique_v(vertexIndices[i] - 1, uvIndices[i] - 1, normalIndices[i] - 1);
        auto it = verticesMap.find(unique_v);
        if (it == verticesMap.end())
        {
            glm::vec3 vertex = temp_vertices[unique_v.v];
            vertices.push_back(vertex);
            glm::vec2 uv = temp_uvs[unique_v.uv];
            uvs.push_back(uv);
            glm::vec3 normal = temp_normals[unique_v.n];
            normals.push_back(normal);
            verticesMap[unique_v] = index;
            indices.push_back(index++);
        }
        else
        {
            indices.push_back(it->second);
        }
    }
    return true;
}