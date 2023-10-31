#pragma once
#include "common.h"

inline glm::vec3 make_orthogonal(glm::vec3 t, glm::vec3 n)
{
    n = normalize(n);
    return t - glm::dot(n, t) * n;
}

void  computeTangentBasis(
    std::vector<uint> &indices,
    std::vector<glm::vec3> &vertices,
    std::vector<glm::vec2> &uvs,
    std::vector<glm::vec3> &normals,
    std::vector<glm::vec3> &tangents,
    std::vector<glm::vec3> &bitangents 
)
{
    tangents = std::vector<glm::vec3>(vertices.size(), glm::vec3(0.f));
    bitangents = std::vector<glm::vec3>(vertices.size(), glm::vec3(0.f));

    for (uint i = 0; i < indices.size(); i += 3)
    {
        uint i0 = indices[i];
        uint i1 = indices[i + 1];
        uint i2 = indices[i + 2];

        glm::vec3 &v0 = vertices[i0];
        glm::vec3 &v1 = vertices[i1];
        glm::vec3 &v2 = vertices[i2];

        glm::vec2 &uv0 = uvs[i0];
        glm::vec2 &uv1 = uvs[i1];
        glm::vec2 &uv2 = uvs[i2];

        glm::vec3 &n0 = normals[i0];
        glm::vec3 &n1 = normals[i1];
        glm::vec3 &n2 = normals[i2];

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        if (glm::dot(glm::cross(n0, tangent), bitangent) < 0.f)
        {
            tangent *= -1.f;
        }

        tangents[i0] += make_orthogonal(tangent, n0);
        tangents[i1] += make_orthogonal(tangent, n1);
        tangents[i2] += make_orthogonal(tangent, n2);

        bitangents[i0] += make_orthogonal(bitangent, n0);
        bitangents[i1] += make_orthogonal(bitangent, n1);
        bitangents[i2] += make_orthogonal(bitangent, n2);
    }
}

