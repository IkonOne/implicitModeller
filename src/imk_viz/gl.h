#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <functional>
#include <string>
#include <vector>

namespace imk_viz {

GLFWwindow* createWindow(int width=800, int height=600, const char* title=nullptr);
void destroyWindow(GLFWwindow* window);
bool windowShouldClose(GLFWwindow* window);
void beginRendering(GLFWwindow* window);
void endRendering(GLFWwindow* window);

const GLuint compileShader(const GLuint type, const char* source);
const void deleteShader(const GLuint shaderID);

const GLuint createProgram(const std::vector<GLuint>& shaderIDs);

} // imk_viz