#version 330 core

in vec2 UV;
in vec3 Pos_world;
in vec3 EyeDir_camera;
in vec3 LightDir_camera;
in vec3 Normal_camera;
in vec3 LightColor;

out vec3 color;

uniform sampler2D sampler;

void main()
{
    vec3 n = normalize(Normal_camera);
    vec3 l = normalize(LightDir_camera);
    vec3 e = normalize(EyeDir_camera);
    vec3 r = reflect(-l, n);

    float cosTheta = clamp(dot(n, l), 0, 1);
    float cosAlpha = clamp(dot(e, r), 0, 1);

    float lobeWidth = 5;

    float lightDist = length(LightDir_camera);

    vec3 materialColor = texture(sampler, UV).rgb;

    vec3 ambientColor = materialColor * vec3(0.1, 0.1, 0.1);
    vec3 diffuseColor = materialColor * LightColor * cosTheta / (lightDist * lightDist);
    vec3 specularColor = materialColor * LightColor * pow(cosAlpha, lobeWidth) / (lightDist * lightDist);

    color = ambientColor + diffuseColor + specularColor;
}