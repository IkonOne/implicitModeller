#include <iostream>
#include <vector>

#include <imk.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

static const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 a_position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(a_position.x, a_position.y, a_position.z, 1.0);\n"
    "}\0";

static const char* fragmentShaderSource = "#version 330 core\n"
    "uniform mat4 u_mvp;\n"
    "uniform vec2 u_resolution;\n"
    "void main()\n"
    "{\n"
    "   vec2 st = gl_FragCoord.xy/u_resolution;\n"
    "   gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);\n"
    "}\n";

int main(void) {
    auto window = imk::gl::createWindow(800, 600, "Minimal Example...");

    auto vertexShader = imk::gl::compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    auto fragmentShader = imk::gl::compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    auto shaderProgram = imk::gl::createProgram({
        vertexShader, fragmentShader
    });
    imk::gl::deleteShader(vertexShader);
    imk::gl::deleteShader(fragmentShader);

    GLint mvp_location, resolution_location;
    mvp_location = glGetUniformLocation(shaderProgram, "u_mvp");
    resolution_location = glGetUniformLocation(shaderProgram, "u_resolution");


    float vertices[] = {
         1.0f,  1.0f, 0.0f,  // top right
         1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    while (!imk::gl::windowShouldClose(window)) {
        imk::gl::beginRendering(window);
        {
            float time = glfwGetTime() * 0.1f;
            glm::mat4 m = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 p = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
            glm::mat4 mvp = m * p;

            glm::ivec2 dims;
            glfwGetFramebufferSize(window, &dims.x, &dims.y);

            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
            glUniform2f(resolution_location, dims.x, dims.y);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        imk::gl::endRendering(window);
    }

    imk::gl::destroyWindow(window);

    return 0;
}