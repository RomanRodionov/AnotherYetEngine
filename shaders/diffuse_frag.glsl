#version 330 core

out vec4 FragColor;

in vec2 UV;

uniform sampler2D diffuse0;
uniform sampler2D emissive0;

void main()
{    
    vec4 diffuse_color = texture(diffuse0, UV);
    vec4 emissive_color = texture(emissive0, UV);
    FragColor = diffuse_color;// + emissive_color;
}