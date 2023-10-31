#version 330 core

layout(location = 0) in vec3 vertexCoords;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec2 UV;
out vec3 Pos_world;
out vec3 EyeDir_camera;
out vec3 LightDir_camera;
out vec3 Normal_camera;
out vec3 LightColor;
out vec3 LightDir_tangent;
out vec3 EyeDir_tangent;

uniform mat4 MVP;
uniform mat4 view;
uniform mat4 model;
uniform mat3 MV3x3;
uniform vec3 lightColor;
uniform float lightPower;
uniform vec3 lightPosWorld;

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

    Normal_camera = MV3x3 * normalize(vertexNormal);
    vec3 Tangent_camera = MV3x3 * normalize(vertexTangent);
    vec3 Bitangent_camera = MV3x3 * normalize(vertexBitangent);

    mat3 TBN = transpose(mat3(
        Tangent_camera,
        Bitangent_camera,
        Normal_camera
    ));

    LightDir_tangent = TBN * LightDir_camera;
    EyeDir_tangent = TBN * EyeDir_camera;
}