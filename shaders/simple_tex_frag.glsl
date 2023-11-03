#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderTex;
uniform float time;

void main(){
    float k = sin(UV.y * 25 + time * 0.5) * 0.1;
    color = texture(renderTex, UV).xyz + vec3(1, 1, 1) * k * k;
}