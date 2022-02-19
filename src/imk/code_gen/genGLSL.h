#pragma once

#include "../csg/csg_node.h"

#include <string>

namespace imk {
namespace code_gen {

std::string genShaderSource(const std::string& getDistFunction);
const std::string genGLSLDistanceFn(const csg::CSGNode& root);

} // namespace code_gen
} // namespace imk