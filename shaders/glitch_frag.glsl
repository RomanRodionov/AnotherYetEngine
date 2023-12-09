#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderTex;
uniform float time;
uniform ivec2 resolution;

//https://github.com/kuravih/gllock/blob/master/shaders/glitch.fragment.glsl

#define RATE 0.75

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453) * 2.0 - 1.0;
}

float offset(float blocks, vec2 uv) {
    float shaderTime = time*RATE;
    return rand(vec2(shaderTime, floor(uv.y * blocks)));
}

void main(){
    color.r = texture(renderTex, UV + vec2(offset(64.0, UV) * 0.03, 0.0)).r;
    color.g = texture(renderTex, UV + vec2(offset(64.0, UV) * 0.03 * 0.16666666, 0.0)).g;
    color.b = texture(renderTex, UV + vec2(offset(64.0, UV) * 0.03, 0.0)).b;
}