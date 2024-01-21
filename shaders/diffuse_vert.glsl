#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 UV;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
    UV = aTexCoords;
    gl_Position = MVP * model * vec4(aPos, 1.0);
}