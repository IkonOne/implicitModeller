#include <iostream>

#include <imk.h>
#include <imk_viz.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

int main(void) {
    auto window = imk::gl::createWindow(800, 600, "CSG Example");
    auto psView = imk::PixelShaderView();

    imk::csg::CSGFactory csg;
    auto tree = csg.Union(
        csg.Union(
            csg.Intersection(
                csg.Sphere({-0.5, -2.5, 6}, 1.0),
                csg.Sphere({-0.0, -2.5, 6}, 1.0)
            ),
            csg.Difference(
                csg.Sphere({-3.5, -2.5, 6}, 1.0),
                csg.Sphere({-3.0, -2.5, 6}, 1.0)
            )
        ),
        csg.Union(
            csg.Union(
                csg.Sphere({3.5, -2.5, 6}, 1.0),
                csg.Sphere({3.0, -2.5, 6}, 1.0)
            ),
            csg.Intersection(
                csg.Union(
                    csg.Difference(
                        csg.Sphere({0,0,50}, 20),
                        csg.Sphere({0,0,50}, 19.8)
                    ),
                    csg.Intersection(
                        csg.Sphere({0,0,50}, 19.8),
                        csg.Gyroid()
                    )
                ),
                csg.Plane({5, 5, 40}, {1,1,-1})
            )
        )
    );

    auto fn = imk::code_gen::genGLSLDistanceFn(*tree);
    std::cout << fn << '\n';

    auto fragSource = imk::code_gen::genShaderSource(fn);
    psView.hotReload(fragSource.c_str());

    while (!imk::gl::windowShouldClose(window)) {
        imk::gl::beginRendering(window);
        {
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);
            psView.resolution({w, h});

            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            psView.mouse({mx, my});

            double time = glfwGetTime();
            psView.time(time);

            psView.draw();
        }
        imk::gl::endRendering(window);
    }

    imk::gl::destroyWindow(window);
}