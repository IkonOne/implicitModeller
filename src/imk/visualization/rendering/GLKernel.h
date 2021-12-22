#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <functional>
#include <string>

namespace imk {

class GLKernel {
public:
    GLKernel() : _window(nullptr) {};
    ~GLKernel() = default;
    GLKernel(const GLKernel&) = delete;

    void create(int width=800, int height=600, const char* title=nullptr);
    void terminate();
    void begin();
    void end();
    const bool shouldClose() const;

public:
    const GLFWwindow* window() const { return this->_window; };

private:
    GLFWwindow* _window;
};

} // imk