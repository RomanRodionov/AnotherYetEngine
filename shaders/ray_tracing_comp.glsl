#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D img_out;

uniform ivec2 resolution;
uniform vec3 position;
uniform vec3 horizontal;
uniform vec3 vertical;
uniform vec3 ll_corner;

float squared_length(vec3 a)
{
    return dot(a, a);
}

const float pos_inf = uintBitsToFloat(0x7F800000);
const float neg_inf = uintBitsToFloat(0xFF800000);
const float PI = 3.14159;
float degrees_to_radians(float degrees)
{
    return degrees * PI / 180.f;
}

struct Ray
{
    vec3 orig;
    vec3 dir; 
};

struct hitRecord
{
    vec3 p;
    vec3 normal;
    double t;
};

struct Interval
{
    float _min, _max;
};

struct Sphere
{
    vec3 center;
    float radius;
};

hitRecord sphere_hit(Sphere s, Ray r, Interval i)
{
    hitRecord hit;
    vec3 oc = r.orig - s.center;
    float a = squared_length(r.dir);
    float hb = dot(oc, r.dir);
    float c = squared_length(oc) - s.radius * s.radius;
    float discr = hb * hb - a * c;
    if (discr < 0) 
    {
        hit.t = -1;
        return hit;
    }
    float sqrtd = sqrt(discr);
    float root = (-hb - sqrtd) / a;
    if (root > i._max)
    {
        hit.t = -1;
        return hit;
    }
    if (root <= i._min)
    {
        root = (-hb + sqrtd) / a;
        if (root <= i._min || root > i._max) 
        {
            hit.t = -1;
            return hit;
        }
    }
    hit.t = root;
    hit.p = r.orig + r.dir * root;
    hit.normal = (hit.p - s.center) / s.radius;
    return hit;
}

void main()
{
    vec4 value = vec4(0, 0, 0, 1.f);
    ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);
    vec2 uv = vec2(texel_coord) / vec2(resolution);

    Ray ray;
    ray.dir = ll_corner + horizontal * uv.x + vertical * uv.y - position;
    ray.orig = position;

    Interval interval = {0.0001, +pos_inf};
    Sphere sphere = {vec3(0.f, 0.f, 0.f), 1.f};

    hitRecord hit = sphere_hit(sphere, ray, interval);
    if (hit.t > 0)
    {
        value = vec4((hit.normal + vec3(1.f, 1.f, 1.f)) / 2.f, 1.f);
    }
    else
    {
        value = vec4(1.f, 1.f, 0.f, 1.f);
    }

    imageStore(img_out, texel_coord, value);
}