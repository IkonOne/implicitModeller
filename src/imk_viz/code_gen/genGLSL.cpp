#include "genGLSL.h"

#include <imk.h>

#include <fmt/core.h>

#include <map>
#include <string>
#include <vector>

namespace imk {
namespace code_gen {

const char* fragHeader = R"glsl(
#version 330 core

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;
uniform mat4 u_mvp;
)glsl";

const char* fragSDFFunctions = R"glsl(
float sdfUnion(float d1, float d2) {
    return min(d1, d2);
}

float sdfDifference(float d1, float d2) {
    return max(d1, -d2);
}

float sdfIntersection(float d1, float d2) {
    return max(d1, d2);
}

float sdfSphere(vec3 point, vec3 position, float radius) {
    return distance(point, position)-radius;
}

float sdfPlane(vec3 point, vec3 position, vec3 normal) {
    normal = normalize(normal);
    return dot(point - position, normal);
}

float sdfGyroid(vec3 point) {
    // FIXME: halving the distance is a hack.
    // There should be an exact solution
    return abs(sin(point.x)*cos(point.y)
            + sin(point.y)*cos(point.z)
            + sin(point.z)*cos(point.x)) * 0.5;
}
)glsl";

const char* calculusFunctions = R"glsl(
// https://www.iquilezles.org/www/articles/normalsSDF/normalsSDF.htm
// Tetrahedron technique.
vec3 calcNormal(in vec3 p) {
    const float h = 0.00001;
    const vec2 k = vec2(1, -1);
    return normalize( k.xyy*getDist( p + k.xyy*h ) + 
                      k.yyx*getDist( p + k.yyx*h ) + 
                      k.yxy*getDist( p + k.yxy*h ) + 
                      k.xxx*getDist( p + k.xxx*h ) );
}
)glsl";

const char* raymarchDefines = R"glsl(
#define MAX_STEPS 100
#define MAX_DISTANCE 100.0
#define SURF_DISTANCE 0.0001
)glsl";

const char* raymarchFunction = R"glsl(
float rayMarch(in vec3 r_origin, in vec3 r_direction) {
    float d_origin = 0.0;
    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 p = r_origin + r_direction * d_origin;
        float d_step = getDist(p);
        if (d_step < SURF_DISTANCE) {
            break;
        }

        d_origin += d_step;

        if (d_origin >= MAX_DISTANCE) {
            d_origin = MAX_DISTANCE;
            break;
        }
    }

    return d_origin;
}
)glsl";

const char* fragMainFunction = R"glsl(
void main() {
    vec2 uv = (gl_FragCoord.xy-0.5*u_resolution) / u_resolution.y;
    vec3 color = vec3(0);

    vec3 r_origin = vec3(0.0, 1.0, 0.0);
    vec3 r_direction = normalize(vec3(uv.x, uv.y, 1.0));

    vec2 mouse = (u_mouse-0.5*u_resolution) / u_resolution.y;
    vec3 rotZ = vec3(-sin(mouse.x), 0.0, cos(mouse.x));
    vec3 rotX = vec3(0.0, sin(mouse.y)*1.3, cos(mouse.y)*1.3);
    // TODO : I never rotate the direction...
    r_origin -= 1.5 * (rotZ + rotX);

    float d = rayMarch(r_origin, r_direction);
    vec3 p = r_origin + r_direction * d;
    vec3 n = calcNormal(p);
    n = n*0.5 + 0.5;
    color = n;

    gl_FragColor = vec4(color, 1.0);
}
)glsl";

std::string genShaderSource(const std::string& getDistFunction) {
    std::string src;
    src += fragHeader;
    src += raymarchDefines;
    src += fragSDFFunctions;
    src += getDistFunction;
    src += calculusFunctions;
    src += raymarchFunction;
    src += fragMainFunction;

    return src;
}

const std::string genGLSLDistanceFn(const csg::CSGNode& root) {
    std::map<csg::CSGNode::Type, std::vector<std::string>> lut = {
        { csg::CSGNode::Type::Union,         { "sdfUnion(",          ", ",   ")" } },
        { csg::CSGNode::Type::Difference,    { "sdfDifference(",     ", ",   ")" } },
        { csg::CSGNode::Type::Intersection,  { "sdfIntersection(",   ", ",   ")" } },

        { csg::CSGNode::Type::Sphere,        { "sdfSphere(point, vec3({px:.4f},{py:.4f},{pz:.4f}), {radius:.4f})" } },
        { csg::CSGNode::Type::Plane,         { "sdfPlane(point, vec3({px:.4f},{py:.4f},{pz:.4f}), vec3({nx:.4f},{ny:.4f},{nz:.4f}))"} },
        { csg::CSGNode::Type::Gyroid,        { "sdfGyroid(point)" } },
    };

    std::string result = "float getDist(in vec3 point) {\n\treturn ";
    for (auto it = root.begin(); it != root.end(); ++it) {
        switch (it->type) {
            case csg::CSGNode::Type::Union:
            case csg::CSGNode::Type::Difference:
            case csg::CSGNode::Type::Intersection:
                result += lut[it->type][it.state()];
                break;
            
            case csg::CSGNode::Type::Complement:
                break;
            
            case csg::CSGNode::Type::Sphere:
                if (it.state() == csg::CSGNode::VisitorIterator::VisitorState::PreOrder) {
                    auto sphere = reinterpret_cast<const csg::CSGSphere&>(it->data());
                    result += fmt::format(lut[it->type][0],
                        fmt::arg("px", sphere.position.x),
                        fmt::arg("py", sphere.position.y),
                        fmt::arg("pz", sphere.position.z),
                        fmt::arg("radius", sphere.radius)
                    );
                }
                break;
            
            case csg::CSGNode::Type::Plane:
                if (it.state() == csg::CSGNode::VisitorIterator::VisitorState::PreOrder) {
                    auto plane = reinterpret_cast<const csg::CSGPlane&>(it->data());
                    result += fmt::format(lut[it->type][0],
                        fmt::arg("px", plane.position.x),
                        fmt::arg("py", plane.position.y),
                        fmt::arg("pz", plane.position.z),
                        fmt::arg("nx", plane.normal.x),
                        fmt::arg("ny", plane.normal.y),
                        fmt::arg("nz", plane.normal.z)
                    );
                }
                break;
            
            case csg::CSGNode::Type::Gyroid:
                if (it.state() == csg::CSGNode::VisitorIterator::VisitorState::PreOrder) {
                    result += lut[it->type][0];
                }
                break;
        }
    }
    result += ";\n}";

    return result;
}

} // namespace code_gen
} // namespace imk