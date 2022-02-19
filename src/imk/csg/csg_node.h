#pragma once

#include <memory> // shared_ptr, enable_shared_from_this

namespace imk {
namespace csg {

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
        Gyroid,

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

        VisitorIterator(pointer ptr, const pointer root, VisitorState state = PreOrder)
            : _ptr(ptr), _root(root), _state(state) {}
        
        const VisitorState state() const;

        reference operator*() const;
        pointer operator->() const;
        VisitorIterator& operator++();
        VisitorIterator operator++(int);

        friend bool operator==(const VisitorIterator& lhs, const VisitorIterator& rhs) { return lhs._ptr == rhs._ptr; }
        friend bool operator!=(const VisitorIterator& lhs, const VisitorIterator& rhs) { return lhs._ptr != rhs._ptr; }

    private:
        VisitorState _state;
        pointer _ptr;
        const pointer _root;
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
        const Type type, CSGNodeData* data,
        const std::shared_ptr<CSGNode>& lhs = nullptr,
        const std::shared_ptr<CSGNode>& rhs = nullptr
    ) {
        auto result = std::shared_ptr<CSGNode>(new CSGNode(type, data));
        if (lhs) result->lhs(lhs);
        if (rhs) result->rhs(rhs);
        return result;
    }

    const Type type;

    iterator begin() const;
    iterator end() const;

    const CSGNodeData& data() const;

    template <class T>
    const T& data() const { return dynamic_cast<T&>(data()); }

    std::shared_ptr<CSGNode> parent() const;

    std::shared_ptr<CSGNode> lhs() const;
    void lhs(const std::shared_ptr<CSGNode>& other);

    std::shared_ptr<CSGNode> rhs() const;
    void rhs(const std::shared_ptr<CSGNode>& other);

private:
    // FIXME: Use get/set to manage parents and the like.
    std::unique_ptr<CSGNodeData> _data;
    std::shared_ptr<CSGNode> _parent;
    std::shared_ptr<CSGNode> _lhs;
    std::shared_ptr<CSGNode> _rhs;

};

} // namespace csg
} // namespace imk