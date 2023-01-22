#include <gtest/gtest.h>

#include <csg/csg_factory.h>
#include <render/marching_cubes.h>

#include <cmath>

TEST(MarchingCubesTests, sphere_points) {
    constexpr float dist = 2.3f;

    imk::csg::CSGFactory csg;
    auto root = csg.Sphere({0, 0, 0}, dist);

    imk::MarchingCubes mc;
    mc.bounds({-3, -3, -3}, {3, 3, 3});
    mc.resolution(0.05);
    const auto [points, tris] = mc.mesh(*root);

    for (int i = 0; i < points.size(); i+=3) {
        const float x = points[i], y = points[i+1], z = points[i+2];
        float mag = std::sqrt(x*x + y*y + z*z);
        EXPECT_NEAR(mag, dist, 0.1);
    }
}