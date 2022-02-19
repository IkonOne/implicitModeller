#include "csg_factory.h"

#include <memory>

namespace imk {
namespace csg {

const std::shared_ptr<CSGNode> CSGFactory::BinaryOp(CSGNode::Type op, const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
    return CSGNode::create(op, nullptr, lhs, rhs);
}

const std::shared_ptr<CSGNode> CSGFactory::Union(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
    return BinaryOp(CSGNode::Type::Union, lhs, rhs);
}

const std::shared_ptr<CSGNode> CSGFactory::Difference(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
    return BinaryOp(CSGNode::Type::Difference, lhs, rhs);
}

const std::shared_ptr<CSGNode> CSGFactory::Intersection(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
    return BinaryOp(CSGNode::Type::Intersection, lhs, rhs);
}

// Unary Ops

const std::shared_ptr<CSGNode> CSGFactory::UnaryOp(CSGNode::Type op, const std::shared_ptr<CSGNode>& lhs) {
    return CSGNode::create(op, nullptr, lhs, nullptr);
}

const std::shared_ptr<CSGNode> CSGFactory::Complement(const std::shared_ptr<CSGNode>& lhs) {
    return UnaryOp(CSGNode::Type::Complement, lhs);
}

// Primitives

const std::shared_ptr<CSGNode> CSGFactory::Primitive(const CSGNode::Type& type, CSGNodeData* data) {
    return CSGNode::create(type, data);
}

const std::shared_ptr<CSGNode> CSGFactory::Plane(const glm::vec3& position, const glm::vec3& normal) {
    return Primitive(CSGNode::Type::Plane, new CSGPlane(position, normal));
}

const std::shared_ptr<CSGNode> CSGFactory::Sphere(const glm::vec3& position, double radius) {
    return Primitive(CSGNode::Type::Sphere, new CSGSphere(position, radius));
}

const std::shared_ptr<CSGNode> CSGFactory::Gyroid() {
    return Primitive(CSGNode::Type::Gyroid, new CSGGyroid());
}

} // namespace csg
} // namespace imk