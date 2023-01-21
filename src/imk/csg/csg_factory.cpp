#include "csg_factory.h"

#include <memory>
#include <utility>

namespace imk {
namespace csg {

std::unique_ptr<CSGNode> CSGFactory::BinaryOp(CSGNode::Type op, std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs) {
    return CSGNode::create(op, nullptr, std::move(lhs), std::move(rhs));
}

std::unique_ptr<CSGNode> CSGFactory::Union(std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs) {
    return BinaryOp(CSGNode::Type::Union, std::move(lhs), std::move(rhs));
}

std::unique_ptr<CSGNode> CSGFactory::Difference(std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs) {
    return BinaryOp(CSGNode::Type::Difference, std::move(lhs), std::move(rhs));
}

std::unique_ptr<CSGNode> CSGFactory::Intersection(std::unique_ptr<CSGNode>&& lhs, std::unique_ptr<CSGNode>&& rhs) {
    return BinaryOp(CSGNode::Type::Intersection, std::move(lhs), std::move(rhs));
}

// Unary Ops

std::unique_ptr<CSGNode> CSGFactory::UnaryOp(CSGNode::Type op, std::unique_ptr<CSGNode>&& lhs) {
    return CSGNode::create(op, nullptr, std::move(lhs));
}

std::unique_ptr<CSGNode> CSGFactory::Complement(std::unique_ptr<CSGNode>&& lhs) {
    return UnaryOp(CSGNode::Type::Complement, std::move(lhs));
}

// Primitives

std::unique_ptr<CSGNode> CSGFactory::Primitive(const CSGNode::Type& type, CSGNodeData* data) {
    return CSGNode::create(type, data);
}

std::unique_ptr<CSGNode> CSGFactory::Plane(const glm::vec3& position, const glm::vec3& normal) {
    return Primitive(CSGNode::Type::Plane, new CSGPlane(position, normal));
}

std::unique_ptr<CSGNode> CSGFactory::Sphere(const glm::vec3& position, double radius) {
    return Primitive(CSGNode::Type::Sphere, new CSGSphere(position, radius));
}

std::unique_ptr<CSGNode> CSGFactory::Gyroid() {
    return Primitive(CSGNode::Type::Gyroid, new CSGGyroid());
}

} // namespace csg
} // namespace imk