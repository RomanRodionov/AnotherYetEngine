#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/glm.hpp"
#include "window.h"
#include "load_shader.h"

#define HEIGHT 1024
#define WIDTH 1536
#define PROJECT_ROOT "/home/roman/Desktop/TinyEngine/"

const double PI = 3.14159;

std::string str_concat(const std::string first, const std::string second);

double clamp(double val, double min_val, double max_val);

#define PATH(file) str_concat(PROJECT_ROOT, file).c_str()

struct Mesh
{
    std::vector<uint> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
};