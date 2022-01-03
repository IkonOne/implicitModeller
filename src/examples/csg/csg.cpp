#include <iostream>

#include <imk.h>

#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <vector>

struct CSGNodeData {};
struct CSGNode 
    : public std::enable_shared_from_this<CSGNode>
{
    enum Type {
        // Binary Ops
        Union = 0,
        Difference,
        Intersection,

        // Unary Ops
        Complement,

        // Primitives
        Sphere,
        Plane,

        // Modifiers
        // RotateEuler,
        // RotateQuaternion,
        // Translate,
        // Scale,
    };

    struct VisitorIterator {
        // Visits all nodes in the subtree depthwise
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = CSGNode;
        using pointer           = CSGNode*;
        using reference         = CSGNode&;

        enum VisitorState {
            PreOrder    = 0,
            InOrder     = 1,
            PostOrder   = 2
        };

        VisitorIterator(pointer ptr, pointer root, VisitorState state = PreOrder)
            : _ptr(ptr), _root(root), _state(state) {}
        
        const VisitorState state() const { return this->_state; }

        reference operator*() const { return *_ptr; }
        pointer operator->() { return _ptr; }

        VisitorIterator& operator++() {
            switch(_state) {
                case PreOrder:
                    if (_ptr->lhs().get()) {
                        _ptr = _ptr->lhs().get();
                    }
                    else {
                        _state = InOrder;
                    }
                    break;

                case InOrder:
                    if (_ptr->rhs().get()) {
                        _state = PreOrder;
                        _ptr = _ptr->rhs().get();
                    }
                    else {
                        _state = PostOrder;
                    }
                    break;

                case PostOrder:
                    pointer parent = _ptr->parent().get();

                    if (parent == nullptr || _ptr == _root) {
                        _ptr = nullptr;
                    }
                    else { // parent exists
                        if (parent->lhs().get() == _ptr) {
                            _state = InOrder;
                        }
                        else {
                            _state = PostOrder;
                        }
                        _ptr = parent;
                    }
                    break;
            }

            return *this;
        }

        VisitorIterator operator++(int) {
            VisitorIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const VisitorIterator& lhs, const VisitorIterator& rhs) { return lhs._ptr == rhs._ptr; }
        friend bool operator!=(const VisitorIterator& lhs, const VisitorIterator& rhs) { return lhs._ptr != rhs._ptr; }

    private:
        VisitorState _state;
        pointer _ptr;
        pointer _root;
    };

    using iterator = VisitorIterator;

    CSGNode() = delete;
    ~CSGNode() = default;
    CSGNode(const CSGNode&) = delete;

private:
    CSGNode(const Type type, CSGNodeData* data)
        : type(type), _data(std::unique_ptr<CSGNodeData>(data)), _lhs(nullptr), _rhs(nullptr), _parent(nullptr)
    { }
    
public:
    [[nodiscard]] static std::shared_ptr<CSGNode> create(
        const Type type, CSGNodeData* data, const std::shared_ptr<CSGNode>& lhs = nullptr, const std::shared_ptr<CSGNode>& rhs = nullptr
    ) {
        auto result = std::shared_ptr<CSGNode>(new CSGNode(type, data));
        if (lhs) result->lhs(lhs);
        if (rhs) result->rhs(rhs);
        return result;
    }

    const Type type;

    iterator begin() { return iterator(this, this); }
    iterator end() { return iterator(nullptr, this); }

    const CSGNodeData& data() const { return *this->_data; }

    template <class T>
    const T& data() const { return dynamic_cast<T&>(data()); }

    std::shared_ptr<CSGNode> parent() const { return this->_parent; }

    std::shared_ptr<CSGNode> lhs() const { return this->_lhs; }
    void lhs(const std::shared_ptr<CSGNode>& other) {
        other->_parent = this->shared_from_this();
        this->_lhs = other;
    }

    std::shared_ptr<CSGNode> rhs() const { return this->_rhs; }
    void rhs(const std::shared_ptr<CSGNode>& other) {
        other->_parent = this->shared_from_this();
        this->_rhs = other;
    }

private:
    // FIXME: Use get/set to manage parents and the like.
    std::unique_ptr<CSGNodeData> _data;
    std::shared_ptr<CSGNode> _parent;
    std::shared_ptr<CSGNode> _lhs;
    std::shared_ptr<CSGNode> _rhs;

};

struct CSGSphere : public CSGNodeData {
    CSGSphere(double radius) : radius(radius) {}
    double radius;
};

struct CSGPlane : public CSGNodeData {
    CSGPlane(const glm::vec3& normal) : normal(normal) {}
    glm::vec3 normal;
};

struct CSGFactory {

    // Binary Ops

    const std::shared_ptr<CSGNode> BinaryOp(CSGNode::Type op, const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
        return CSGNode::create(op, nullptr, lhs, rhs);
    }

    const std::shared_ptr<CSGNode> Union(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
        return BinaryOp(CSGNode::Type::Union, lhs, rhs);
    }

    const std::shared_ptr<CSGNode> Difference(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
        return BinaryOp(CSGNode::Type::Difference, lhs, rhs);
    }

    const std::shared_ptr<CSGNode> Intersection(const std::shared_ptr<CSGNode>& lhs, const std::shared_ptr<CSGNode>& rhs) {
        return BinaryOp(CSGNode::Type::Intersection, lhs, rhs);
    }

    // Unary Ops

    const std::shared_ptr<CSGNode> UnaryOp(CSGNode::Type op, const std::shared_ptr<CSGNode>& lhs) {
        return CSGNode::create(op, nullptr, lhs, nullptr);
    }

    const std::shared_ptr<CSGNode> Complement(const std::shared_ptr<CSGNode>& lhs) {
        return UnaryOp(CSGNode::Type::Complement, lhs);
    }

    // Primitives

    const std::shared_ptr<CSGNode> Primitive(const CSGNode::Type& type, CSGNodeData* data) {
        return CSGNode::create(type, data);
    }

    const std::shared_ptr<CSGNode> Plane(const glm::vec3& normal) {
        return Primitive(CSGNode::Type::Plane, new CSGPlane(normal));
    }

    const std::shared_ptr<CSGNode> Sphere(double radius) {
        return Primitive(CSGNode::Type::Sphere, new CSGSphere(radius));
    }
};

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
            + sin(point.z)*cos(point.x)) * 0.6;
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
#define SURF_DISTANCE 0.001
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

int main(void) {
    CSGFactory csg;
    auto tree = csg.Union(
        csg.Sphere(2.0),
        csg.Sphere(2.0)
    );


    int i = 0;
    for (auto it = tree->begin(); it != tree->end(); ++it) {
        std::cout << it.state() << ", " << i++ << '\n';
    }

    auto window = imk::gl::createWindow(800, 600, "CSG Example");
    auto psView = imk::PixelShaderView();

    auto fragSource = genShaderSource(R"glsl(
        float getDist(in vec3 p) {
            float s1 = sdfSphere(p, vec3(-0.5,-2.5,6), 1);
            float s2 = sdfSphere(p, vec3(0.5,-2.5,6), 1);
            float i = sdfIntersection(s2, s1);

            float s3 = sdfSphere(p, vec3(-0.5,0,6), 1);
            float s4 = sdfSphere(p, vec3(0.5,0,6), 1);
            float d = sdfDifference(s3, s4);

            float s5 = sdfSphere(p, vec3(-0.5,2.5,6), 1);
            float s6 = sdfSphere(p, vec3(0.5,2.5,6), 1);
            float u = sdfUnion(s5, s6);

            float g1 = sdfGyroid(p);
            float p1 = sdfPlane(p, vec3(0,0,30), vec3(0,0,-1));
            float g = sdfIntersection(g1,p1);

            return sdfUnion(sdfUnion(sdfUnion(i, d), u), g);
        }
    )glsl");

    psView.hotReload(fragSource.c_str());

    while (!imk::gl::windowShouldClose(window)) {
        imk::gl::beginRendering(window);
        {
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);
            psView.resolution({w, h});

            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            psView.mouse({mx, my});

            double time = glfwGetTime();
            psView.time(time);

            psView.draw();
        }
        imk::gl::endRendering(window);
    }

    imk::gl::destroyWindow(window);
}