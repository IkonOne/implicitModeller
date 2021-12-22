#include "GLKernel.h"

#include <iostream>

namespace imk {

void GLKernel::create(int width, int height, const char* title) {
    auto error_cb = [](int error, const char* description) {
        std::cerr << "[ERROR] GLFW : (" << error << ") " << description << '\n';
    };

    glfwSetErrorCallback(error_cb);

    if (!glfwInit()) {
        throw std::runtime_error("[GLFW] Failed to initialize glfw...");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    // glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
    this->_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!this->_window) {
        glfwTerminate();
        throw std::runtime_error("[GLFW] Failed to create glfw window...");
    }

    auto key_cb = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    };
    glfwSetKeyCallback(this->_window, key_cb);

    glfwMakeContextCurrent(this->_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // glfwSetWindowSize(this->_window, width, height);
}

void GLKernel::terminate() {
    glfwDestroyWindow(this->_window);
    glfwTerminate();

    this->_window = nullptr;
}

void GLKernel::begin() {
    glm::ivec2 fb_dims;
    glfwGetFramebufferSize(this->_window, &fb_dims.x, &fb_dims.y);

    glViewport(0, 0, fb_dims.x, fb_dims.y);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLKernel::end() {
    glfwSwapBuffers(this->_window);
    glfwPollEvents();
}

const bool GLKernel::shouldClose() const {
    return glfwWindowShouldClose(this->_window);
}

} // imk