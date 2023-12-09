#pragma once
#include "common.h"

class Renderable
{
public:
    virtual bool draw() const = 0;
};

class StandartMesh: public Renderable
{
    Mesh *mesh;
    GLuint diffuseTex;
    GLuint normalTex;
    GLuint roughTex;
    GLuint aoTex;
    GLuint programID;
    GLuint diffuseTexID;
    GLuint normalTexID;
    GLuint roughTexID;
    GLuint aoTexID;
    glm::mat4 transform;
public:
    StandartMesh(
        Mesh *_mesh,
        GLuint _diffuseTex,
        GLuint _normalTex,
        GLuint _roughTex,
        GLuint _aoTex,
        GLuint _programID
    )
    {
        mesh       = _mesh;
        diffuseTex = _diffuseTex;
        normalTex  = _normalTex;
        roughTex   = _roughTex;
        aoTex      = _aoTex;
        programID  = _programID;
        diffuseTexID = glGetUniformLocation(programID, "diffuseTexSampler");
        normalTexID  = glGetUniformLocation(programID, "normalTexSampler");
        roughTexID   = glGetUniformLocation(programID, "roughTexSampler");
        aoTexID      = glGetUniformLocation(programID, "aoTexSampler");
        transform = glm::mat4(1.f);
    }
};