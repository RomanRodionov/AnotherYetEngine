#version 330 core

layout(location = 0) out vec3 color;

in vec2 UV;
in vec3 Pos_world;
in vec3 EyeDir_camera;
in vec3 LightDir_camera;
in vec3 Normal_camera;
in vec3 LightColor;
in vec3 LightDir_tangent;
in vec3 EyeDir_tangent;

uniform sampler2D diffuseTexSampler;
uniform sampler2D normalTexSampler;
uniform sampler2D roughTexSampler;
uniform sampler2D aoTexSampler;

void main()
{
    vec3 Normal_tangent = normalize(texture(normalTexSampler, UV).rgb * 2.f - 1.f);
    vec3 materialColor = texture(diffuseTexSampler, UV).rgb;
    vec3 roughness = texture(roughTexSampler, UV).rgb;
    vec3 ambientOcclusion = texture(aoTexSampler, UV).rgb;

    vec3 n = normalize(Normal_tangent);
    vec3 l = normalize(LightDir_tangent);
    vec3 e = normalize(EyeDir_tangent);
    vec3 r = reflect(-l, n);

    float cosTheta = clamp(dot(n, l), 0, 1);
    float cosAlpha = clamp(dot(e, r), 0, 1);

    float lobeWidth = 5;

    float lightDist = length(LightDir_camera);

    vec3 ambientColor = materialColor;
    vec3 specularColor = (vec3(1.f, 1.f, 1.f) - roughness) * 0.5;
    vec3 diffuseColor = materialColor;

    vec3 ambientLight = ambientColor * vec3(0.1, 0.1, 0.1);
    vec3 diffuseLight = diffuseColor * LightColor * cosTheta / (lightDist * lightDist);
    vec3 specularLight = specularColor * LightColor * pow(cosAlpha, lobeWidth) / (lightDist * lightDist);

    color = (ambientLight + diffuseLight + specularLight) * ambientOcclusion;
}