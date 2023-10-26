#version 330 core

layout(location = 0) in vec3 vertexCoords;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexCoords, 1);
    UV = vertexUV;
}