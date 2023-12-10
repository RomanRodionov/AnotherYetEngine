#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D img_out;

uniform ivec2 resolution;
uniform vec3 position;
uniform vec3 horizontal;
uniform vec3 vertical;
uniform vec3 ll_corner;
uniform float seed;

const uint iterations = 50;

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
float rand(vec2 s){
    float res = fract(sin(dot(s, vec2(12.9898, 78.233))) * 43758.5453);
    return res - 0.5;
}
vec3 random_in_unit_sphere()
{
    vec2 k = fract(seed + vec2(gl_GlobalInvocationID.xy) / vec2(resolution));
    while (true)
    {
        vec3 point;
        point.x = rand(k);
        k = fract(k * PI);
        point.y = rand(k);
        k = fract(k * PI);
        point.z = rand(k);
        k = fract(k * PI);
        if (squared_length(point) < 1.f) 
        {
            return point;
        }
    }
}

vec3 random_unit_vector()
{
    return normalize(random_in_unit_sphere());
}

struct Ray
{
    vec3 orig;
    vec3 dir; 
    vec3 color;
};

struct hitRecord
{
    vec3 p;
    vec3 normal;
    float t;
    uint mat;
};

struct Interval
{
    float _min, _max;
};

struct Sphere
{
    vec3 center;
    float radius;
    uint mat;
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
    hit.mat = s.mat;
    return hit;
}

Ray scatterLambertian(
    Ray ray, 
    hitRecord hit, 
    vec3 attenuation
)
{
    vec3 scatter_direction = hit.normal + random_unit_vector();
    if (length(scatter_direction) < 0.0001)
    {
        scatter_direction = hit.normal;
    }
    return Ray(hit.p, scatter_direction, ray.color * attenuation);
}

void main()
{
    vec4 value = vec4(0, 0, 0, 1.f);
    ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);
    vec2 uv = vec2(texel_coord) / vec2(resolution);

    Ray ray;
    ray.dir = ll_corner + horizontal * uv.x + vertical * uv.y - position;
    ray.orig = position;
    ray.color = vec3(1.f, 1.f, 1.f);

    Sphere sphere1 = {vec3(0.f, 0.f, 0.f), 1.f, 0};
    Sphere sphere2 = {vec3(0.f, -101.f, 0.f), 100.f, 1};
    Sphere spheres[2] = {sphere1, sphere2};
    hitRecord hit;
    bool miss = false;

    for (int it = 0; it < iterations && !miss; ++it)
    {
        Interval interval = {0.0001, +pos_inf};
        hit.t = -1;
        for (int obj = 0; obj < 2; ++obj)
        {
            hitRecord new_hit = sphere_hit(spheres[obj], ray, interval);
            if (new_hit.t > 0.f && new_hit.t < interval._max)
            {
                interval._max = new_hit.t;
                hit = new_hit;
            }
        }
        if (hit.t > 0)
        {
            if (hit.mat == 0)
                ray = scatterLambertian(ray, hit, vec3(0.1, 0.5, 0.5));
            else
                ray = scatterLambertian(ray, hit, vec3(0.5, 0.1, 0.5));
        }
        else
        {
            miss = true;
        }
    }
    if (miss)
    {
        value = vec4(ray.color * vec3(1.f, 1.f, 1.f), 1.f);
    }

    imageStore(img_out, texel_coord, value);
}