#version 330 core

in vec2 UV;
in vec3 Pos_world;
in vec3 EyeDir_camera;
in vec3 LightDir_camera;
in vec3 Normal_camera;
in vec3 LightColor;
in vec3 LightDir_tangent;
in vec3 EyeDir_tangent;

out vec3 color;

uniform sampler2D diffuseTexSampler;
uniform sampler2D normalTexSampler;

void main()
{
    vec3 Normal_tangent = normalize(texture(normalTexSampler, UV).rgb * 2.f - 1.f);
    vec3 materialColor = texture(diffuseTexSampler, UV).rgb;

    vec3 n = normalize(Normal_tangent);
    vec3 l = normalize(LightDir_tangent);
    vec3 e = normalize(EyeDir_tangent);
    vec3 r = reflect(-l, n);

    float cosTheta = clamp(dot(n, l), 0, 1);
    float cosAlpha = clamp(dot(e, r), 0, 1);

    float lobeWidth = 5;

    float lightDist = length(LightDir_camera);

    float specular = 0.2;
    float diffuse = 1.0 - specular;

    vec3 ambientColor = materialColor * vec3(0.1, 0.1, 0.1);
    vec3 diffuseColor = materialColor * LightColor * cosTheta / (lightDist * lightDist) * diffuse;
    vec3 specularColor = LightColor * pow(cosAlpha, lobeWidth) / (lightDist * lightDist) * specular;

    color = ambientColor + diffuseColor + specularColor;
}