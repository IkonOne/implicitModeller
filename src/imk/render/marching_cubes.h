#pragma once
#ifndef __IMK_RENDER_BREP_MARCHING_CUBES_H__

#include <csg/csg_node.h>

#include <glm/vec3.hpp>

#include <utility>
#include <vector>

namespace imk {

class MarchingCubes {
    std::pair<glm::vec3, glm::vec3> _bounds;
    float _resolution;

public:
    MarchingCubes();
    ~MarchingCubes() = default;
    MarchingCubes(const MarchingCubes&) = default;

    std::pair<glm::vec3, glm::vec3> bounds() const { return _bounds; }
    void bounds(const glm::vec3 min, const glm::vec3 max);

    float resolution() const { return _resolution; }
    void resolution(const float resolution);

    std::pair<std::vector<float>, std::vector<int>> mesh(const imk::csg::CSGNode& root) const;
};

}

#endif // __IMK_RENDER_BREP_MARCHING_CUBES_H__