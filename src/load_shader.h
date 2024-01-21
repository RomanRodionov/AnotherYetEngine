#pragma once

#include "common.h"

class Shader
{
    GLuint programID;
public:
    Shader() {}
    ~Shader()
    {
        glDeleteProgram(programID);
    }
    void use()
    {
        glUseProgram(programID);
    }
    GLuint getProgramID() const
    {
        return programID;
    }
    void setProgramID(GLuint id)
    {
        programID = id;
    }
    GLuint getAttribLocation(const std::string &name)
    {
        return glGetAttribLocation(programID, name.c_str());
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(programID, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value); 
    }
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]); 
    }
    void setIvec2(const std::string &name, const glm::ivec2 &value) const
    { 
        glUniform2iv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]); 
    }
    void setMat3(const std::string &name, const glm::mat3 &value)
    {
        glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
    void setMat4(const std::string &name, const glm::mat4 &value)
    {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
};

class VertFragShader : public Shader
{
public:
    VertFragShader(const char* vertex_file_path, const char* fragment_file_path);
};

class ComputeShader : public Shader
{
public:
    ComputeShader(const char* shader_file_path);
};