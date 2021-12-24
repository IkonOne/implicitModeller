#include "gl.h"

#include <iostream>
#include <string>

char infoLog[512];

namespace imk {
namespace gl {

GLFWwindow* createWindow(int width, int height, const char* title) {
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

    auto windowID = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!windowID) {
        glfwTerminate();
        throw std::runtime_error("[GLFW] Failed to create glfw window...");
    }

    auto key_cb = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    };
    glfwSetKeyCallback(windowID, key_cb);

    glfwMakeContextCurrent(windowID);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    return windowID;
}

bool windowShouldClose(GLFWwindow* windowID) {
    return glfwWindowShouldClose(const_cast<GLFWwindow*>(windowID));
}

void destroyWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
}

void beginRendering(GLFWwindow* window) {
    glm::ivec2 fb_dims;
    glfwGetFramebufferSize(window, &fb_dims.x, &fb_dims.y);

    glViewport(0, 0, fb_dims.x, fb_dims.y);
    glClear(GL_COLOR_BUFFER_BIT);
}

void endRendering(GLFWwindow* window) {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

const GLuint compileShader(const GLuint type, const char* source) {
    GLuint shaderID = 0;

    shaderID = glCreateShader(type);
    if (shaderID == GL_NONE) {
        throw std::runtime_error("Failed to create shader...");
        return GL_NONE;
    }

    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::string err = "Failed to compile shader...\n";
        err += infoLog;
        throw std::runtime_error(err.c_str());
        return GL_NONE;
    }

    return shaderID;
}

const void deleteShader(const GLuint shaderID) {
    glDeleteShader(shaderID);
}

const GLuint createProgram(const std::vector<GLuint>& shaderIDs) {
    int success;
    GLuint programID;

    programID = glCreateProgram();
    if (programID == GL_NONE) {
        throw std::runtime_error("Faield to create shader program...");
        return GL_NONE;
    }

    for (auto shaderID : shaderIDs) {
        glAttachShader(programID, shaderID);
    }

    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::string err = "Failed to link shader program...\n";
        err += infoLog;
        throw std::runtime_error(err.c_str());
        return GL_NONE;
    }

    return programID;
}

}
} // imk