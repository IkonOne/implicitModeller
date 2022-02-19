#pragma once

#include "csg_node.h"
#include "csg_nodes.h"

namespace imk {
namespace csg {

struct CSGFactory {

    // Binary Ops

    const std::shared_ptr<CSGNode> BinaryOp(CSGNode::Type op, const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs);
    const std::shared_ptr<CSGNode> Union(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs);
    const std::shared_ptr<CSGNode> Difference(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs);
    const std::shared_ptr<CSGNode> Intersection(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs);

    // Unary Ops
    const std::shared_ptr<CSGNode> UnaryOp(CSGNode::Type op, const std::shared_ptr<CSGNode>& lhs);
    const std::shared_ptr<CSGNode> Complement(const std::shared_ptr<CSGNode>& lhs);

    // Primitives
    const std::shared_ptr<CSGNode> Primitive(const CSGNode::Type& type, CSGNodeData* data);
    const std::shared_ptr<CSGNode> Plane(const glm::vec3& position, const glm::vec3& normal);
    const std::shared_ptr<CSGNode> Sphere(const glm::vec3& position, double radius);
    const std::shared_ptr<CSGNode> Gyroid();
};

} // namespace csg
} // namespace imk