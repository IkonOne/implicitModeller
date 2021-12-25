#pragma once

#include <imk.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

class PixelShaderView {
public:
    PixelShaderView();
    ~PixelShaderView();
    PixelShaderView(const PixelShaderView&) = delete;

    const std::string fragmentShaderSource() const { return this->_fragSource; }
    const std::string vertexShaderSource() const { return this->_vertSource; }

    void draw() const;

    void hotReload(const char* vertSource);

    void resolution(const glm::vec2& resolution) {
        this->_resolution = resolution;
    }

    void mvp(const glm::mat4& mvp) {
        this->_mvp = mvp;
    }

    void time(const float time) {
        this->_time = time;
    }

    void mouse(const glm::vec2& mouse) {
        this->_mouse = mouse;
    }

private:
    GLint _buildShaderProgram();

    std::string _fragSource;
    std::string _vertSource;

    GLint _program;
    GLuint _VBO, _VAO, _EBO;

    GLint _u_resolution;
    glm::vec2 _resolution;

    GLint _u_mvp;
    glm::mat4 _mvp;

    GLint _u_time;
    float _time;

    GLint _u_mouse;
    glm::vec2 _mouse;
};