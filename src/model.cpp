#include "model.h"

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
    glm::mat4 to;

    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

    return to;
}

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
    //std::cout << textures.size() << std::endl;
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
    shader.setMat4("model", m_transform);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertexArrayID);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    for (uint i = 0; i < textures.size(); ++i)
    {
        textures[i].unbind();
    }
    glBindVertexArray(0);
}

void Model::draw(Shader& shader)
{
    //std::cout << meshes.size() << std::endl;
    for (uint i = 0; i < meshes.size(); ++i)
    {
        meshes[i].draw(shader);
    }
}