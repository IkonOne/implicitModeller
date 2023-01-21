#pragma once

#include "csg_node.h"
#include "csg_nodes.h"

namespace imk {
namespace csg {

struct CSGFactory {

    // Binary Ops

    std::unique_ptr<CSGNode> BinaryOp(CSGNode::Type op, std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs);
    std::unique_ptr<CSGNode> Union(std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs);
    std::unique_ptr<CSGNode> Difference(std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs);
    std::unique_ptr<CSGNode> Intersection(std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs);

    // Unary Ops
    std::unique_ptr<CSGNode> UnaryOp(CSGNode::Type op, std::unique_ptr<CSGNode>&& lhs);
    std::unique_ptr<CSGNode> Complement(std::unique_ptr<CSGNode>&& lhs);

    // Primitives
    std::unique_ptr<CSGNode> Primitive(const CSGNode::Type& type, CSGNodeData* data);
    std::unique_ptr<CSGNode> Plane(const glm::vec3& position, const glm::vec3& normal);
    std::unique_ptr<CSGNode> Sphere(const glm::vec3& position, double radius);
    std::unique_ptr<CSGNode> Gyroid();
};

} // namespace csg
} // namespace imk