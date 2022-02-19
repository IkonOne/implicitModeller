#pragma once

#include "csg_node.h"

#include <glm/glm.hpp>

namespace imk {
namespace csg {

struct CSGSphere : public CSGNodeData {
    CSGSphere(const glm::vec3& position, double radius)
        : position(position), radius(radius) {}
    const glm::vec3 position;
    double radius;
};

struct CSGPlane : public CSGNodeData {
    CSGPlane(const glm::vec3& position, const glm::vec3& normal)
        : position(position), normal(normal) {}
    glm::vec3 position;
    glm::vec3 normal;
};

struct CSGGyroid : public CSGNodeData {};

} // namespace csg
} // namespace imk