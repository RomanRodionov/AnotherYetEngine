#version 330 core

layout(location = 0) in vec3 vertexCoords;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 Pos_world;
out vec3 EyeDir_camera;
out vec3 LightDir_camera;
out vec3 Normal_camera;
out vec3 LightColor;


uniform mat4 MVP;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightColor;
uniform float lightPower;
uniform vec3 lightPosWorld;

// W... - world space
// C... - camera space

void main()
{
    gl_Position = MVP * vec4(vertexCoords, 1);
    UV = vertexUV;
    LightColor = lightColor * lightPower;

    Pos_world = (model * vec4(vertexCoords, 1)).xyz;

    vec3 Pos_camera = (view * model * vec4(vertexCoords, 1)).xyz;
    EyeDir_camera = vec3(0, 0, 0) - Pos_camera;
    
    vec3 LightPos_camera = (view * vec4(lightPosWorld, 1)).xyz;
    LightDir_camera = LightPos_camera + EyeDir_camera;

    Normal_camera = (view * model * vec4(vertexNormal, 0)).xyz;
}