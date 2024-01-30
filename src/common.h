#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/glm.hpp"
#include "window.h"
#include "load_shader.h"

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1400
#define PROJECT_ROOT "/home/roman/Desktop/TinyEngine/"

const double PI = 3.14159;

std::string str_concat(const std::string first, const std::string second);

double clamp(double val, double min_val, double max_val);

#define PATH(file) str_concat(PROJECT_ROOT, file).c_str()

#define uchar unsigned char