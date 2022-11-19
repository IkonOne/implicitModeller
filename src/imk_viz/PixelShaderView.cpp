#include "PixelShaderView.h"
#include "gl.h"

#include <imk.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>

namespace imk_viz {

static const char* vertSource = "#version 330 core\n"
    "layout (location = 0) in vec3 a_position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(a_position.x, a_position.y, a_position.z, 1.0);\n"
    "}\0";

static const char* fragSource = "#version 330 core\n"
    "uniform mat4 u_mvp;\n"
    "uniform vec2 u_resolution;\n"
    "void main()\n"
    "{\n"
    "   vec2 st = gl_FragCoord.xy/u_resolution;\n"
    "   gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);\n"
    "}\n";

static float vertices[] = {
     1.0f,  1.0f, 0.0f,  // top right
     1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f   // top left 
};
static unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
}; 

PixelShaderView::PixelShaderView()
    : _mvp(glm::mat4(1.0))
{
    this->_vertSource = vertSource;
    this->_fragSource = fragSource;
    this->_program = this->_buildShaderProgram();

    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);
    glGenBuffers(1, &this->_EBO);
    
    glBindVertexArray(this->_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

PixelShaderView::~PixelShaderView() {
    glDeleteVertexArrays(1, &this->_VAO);
    glDeleteBuffers(1, &this->_VBO);
    glDeleteBuffers(1, &this->_EBO);
    glDeleteProgram(this->_program);
}

void PixelShaderView::draw() const {
    glUseProgram(this->_program);
    glBindVertexArray(this->_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);

    glUniform2f(this->_u_resolution, this->_resolution.x, this->_resolution.y);
    glUniformMatrix4fv(this->_u_mvp, 1, GL_FALSE, glm::value_ptr(this->_mvp));
    glUniform1f(this->_u_time, this->_time);
    glUniform2f(this->_u_mouse, this->_mouse.x, this->_mouse.y);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLint PixelShaderView::_buildShaderProgram() {
    GLint program;

    auto vertShader = imk_viz::compileShader(GL_VERTEX_SHADER, this->_vertSource.c_str());
    auto fragShader = imk_viz::compileShader(GL_FRAGMENT_SHADER, this->_fragSource.c_str());
    program = imk_viz::createProgram({vertShader, fragShader});
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    this->_u_resolution = glGetUniformLocation(program, "u_resolution");
    this->_u_mvp = glGetUniformLocation(program, "u_mvp");
    this->_u_time = glGetUniformLocation(program, "u_time");
    this->_u_mouse = glGetUniformLocation(program, "u_mouse");

    return program;
}

void PixelShaderView::hotReload(const char* fragSource) {
    this->_fragSource = fragSource;
    auto program = this->_buildShaderProgram();

    glDeleteProgram(this->_program);
    this->_program = program;
}

} // namespace imk_viz