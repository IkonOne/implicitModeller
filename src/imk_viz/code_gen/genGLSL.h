#pragma once

// #include "../csg/csg_node.h"
#include <imk.h>

#include <string>

namespace imk_viz {

std::string genShaderSource(const std::string& getDistFunction);
const std::string genGLSLDistanceFn(const imk::csg::CSGNode& root);

} // namespace imk