#include "load_shader.h"
#include <fstream>
#include <sstream>
#include <vector>

VertFragShader::VertFragShader(const char* vertex_file_path, const char* fragment_file_path)
{
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else
    {
        std::cerr << "Impossible to open " << std::string(vertex_file_path) << std::endl;
        return;
    }

    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    std::cout << "Compiling shader: " << std::string(vertex_file_path) << std::endl;
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        std::cerr << std::string(&VertexShaderErrorMessage[0]) << std::endl;
    }

    std::cout << "Compiling shader: " << std::string(fragment_file_path) << std::endl;
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        std::cerr << std::string(&FragmentShaderErrorMessage[0]) << std::endl;
    }

    std::cout << "Linking program" << std::endl;
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::cerr << std::string(&ProgramErrorMessage[0]) << std::endl;
    }
    setProgramID(ProgramID);
    
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}

ComputeShader::ComputeShader(const char* shader_file_path)
{
    GLuint ShaderID = glCreateShader(GL_COMPUTE_SHADER);

    std::string ShaderCode;
    std::ifstream ShaderStream(shader_file_path, std::ios::in);
    if (ShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderCode = sstr.str();
        ShaderStream.close();
    }
    else
    {
        std::cerr << "Impossible to open " << std::string(shader_file_path) << std::endl;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    std::cout << "Compiling shader: " << std::string(shader_file_path) << std::endl;
    char const* SourcePointer = ShaderCode.c_str();
    glShaderSource(ShaderID, 1, &SourcePointer, NULL);
    glCompileShader(ShaderID);

    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        std::cerr << std::string(&ShaderErrorMessage[0]) << std::endl;
    }

    std::cout << "Linking program" << std::endl;
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, ShaderID);
    glLinkProgram(ProgramID);

    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::cerr << std::string(&ProgramErrorMessage[0]) << std::endl;
    }
    setProgramID(ProgramID);

    glDetachShader(ProgramID, ShaderID);
    glDeleteShader(ShaderID);
}