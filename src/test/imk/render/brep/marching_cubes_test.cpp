#include <gtest/gtest.h>

#include <imk.h>

TEST(MarchingCubesTests, sphere_points) {
    constexpr float dist = 2.3f;

    imk::csg::CSGFactory csg;
    auto root = csg.Sphere({0, 0, 0}, dist);

    imk::render::brep::MarchingCubes mc;
    mc.bounds({-3, -3, -3}, {3, 3, 3});
    mc.resolution(0.05);
    const auto [points, tris] = mc.mesh(root);

    for (const auto&& pt : points) {
        EXPECT_NEAR(pt.magnitude(), dist, 0.1f);
    }
}