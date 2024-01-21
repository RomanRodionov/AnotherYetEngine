#include "model.h"

void Mesh::setup()
{
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    // vertex position 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex uv coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

void Mesh::draw(Shader& shader)
{
    std::array<uint, static_cast<size_t>(TexType::SIZE)> count = {0};
    for (uint i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name;
        TexType type = textures[i].getType();
        switch (type)
        {
            case TexType::NONE:
                name = "none" + std::to_string(count[0]++);
                break;
            case TexType::DIFFUSE:
                name = "diffuse" + std::to_string(count[1]++);
                break;
            case TexType::SPECULAR:
                name = "specular" + std::to_string(count[2]++);
                break;
            case TexType::AMBIENT:
                name = "ambient" + std::to_string(count[3]++);
                break;
            case TexType::EMISSIVE:
                name = "emissive" + std::to_string(count[4]++);
                break;
            case TexType::HEIGHT:
                name = "height" + std::to_string(count[5]++);
                break;
            case TexType::NORMALS:
                name = "normals" + std::to_string(count[6]++);
                break;
            case TexType::SHININESS:
                name = "shininess" + std::to_string(count[7]++);
                break;
            case TexType::OPACITY:
                name = "opacity" + std::to_string(count[8]++);
                break;
            case TexType::DISPLACEMENT:
                name = "displacement" + std::to_string(count[9]++);
                break;
            case TexType::LIGHTMAP:
                name = "lightmap" + std::to_string(count[10]++);
                break;
        }
        shader.setInt(name.c_str(), i);
        textures[i].bind();
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertexArrayID);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::draw(Shader& shader)
{
    for (uint i = 0; i < meshes.size(); ++i)
    {
        meshes[i].draw(shader);
    }
}