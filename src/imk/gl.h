#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <functional>
#include <string>
#include <vector>

namespace imk {
namespace gl {

using GLShaderID = GLuint;
using GLProgramID = GLuint;

GLFWwindow* createWindow(int width=800, int height=600, const char* title=nullptr);
void destroyWindow(GLFWwindow* window);
bool windowShouldClose(GLFWwindow* window);
void beginRendering(GLFWwindow* window);
void endRendering(GLFWwindow* window);

const GLShaderID compileShader(const GLuint type, const char* source);
const void deleteShader(const GLShaderID shaderID);

const GLProgramID createProgram(const std::vector<GLShaderID>& shaderIDs);

} // gl
} // imk