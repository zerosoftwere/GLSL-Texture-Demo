#pragma once

#include <Windows.h>

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#define PI 3.14159f

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef GLSL
#define GLSL(version, code) "#version " #version "\n" #code
#endif

void ShaderCompileCheck(GLuint id);
void ShaderLinkCheck(GLuint id);
GLuint LoadShaders(const char* vert, const char* frag);
GLuint LoadTexture(const char* filename);

template<typename T>
T ExtractValue(unsigned char* data, unsigned int pos);