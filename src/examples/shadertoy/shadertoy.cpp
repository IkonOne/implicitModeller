// #include "PixelShaderView.h"

#include <imk.h>
#include <imk_viz.h>

#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string loadTextFile(const char* filename) {
    std::ifstream in(filename);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

int main(int argc, char* argv[]) {
    auto window = imk_viz::createWindow(800, 600, "Shadertoy");
    imk_viz::PixelShaderView psView;

    imk::FileWatcher fileShader;
    if (argc > 1) {
        fileShader.start(argv[1]);
        auto fragShaderSource = loadTextFile(fileShader.path().c_str());
        std::cout << "Loading shader : " << argv[1] << '\n';
        psView.hotReload(fragShaderSource.c_str());
        std::cout << "loaded...\n";
    }

    while (!glfwWindowShouldClose(window)) {
        if (fileShader.isWatching() && fileShader.checkModified()) {
            auto fragShaderSource = loadTextFile(fileShader.path().c_str());

            try {
                std::cout << "Reloading shader : " << fileShader.path() << '\n';
                psView.hotReload(fragShaderSource.c_str());
            }
            catch (const std::runtime_error& e) {
                std::cout << "Failed to reload shader...\n";
                std::cout << e.what() << '\n';
            }
        }

        imk_viz::beginRendering(window);
        {
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);
            psView.resolution({w, h});

            auto elapsed = glfwGetTime();
            psView.time(elapsed);

            glm::dvec2 mousePos;
            glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
            psView.mouse(mousePos);

            psView.draw();
        }
        imk_viz::endRendering(window);
    }

    glfwDestroyWindow(window);
    
    return 0;
}