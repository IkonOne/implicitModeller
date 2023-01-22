#pragma once

#include <memory> // unique_ptr
#include <utility> // move

namespace imk {
namespace csg {

struct CSGNodeData {};

struct CSGNode {
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

        EMPTY,
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
            : _ptr(ptr),
            _state(state),
            _root(root)
        {}
        
        const VisitorState state() const;

        reference operator*() const;
        pointer operator->() const;
        VisitorIterator& operator++();
        VisitorIterator operator++(int);

        friend bool operator==(const VisitorIterator& lhs, const VisitorIterator& rhs) { return lhs._ptr == rhs._ptr; }
        friend bool operator!=(const VisitorIterator& lhs, const VisitorIterator& rhs) { return lhs._ptr != rhs._ptr; }

    private:
        pointer _ptr;
        VisitorState _state;
        const pointer _root;
    };
    
    using iterator = VisitorIterator;

    ~CSGNode() = default;
    CSGNode(const CSGNode&);

private:
    CSGNode() = default;
    CSGNode(const Type type, CSGNodeData* data)
        : type(type),
        _data(data),
        _parent(nullptr),
        _lhs(nullptr),
        _rhs(nullptr)
    { }
    
public:
    [[nodiscard]] static std::unique_ptr<CSGNode> create(const Type type, CSGNodeData* data) {
        return std::unique_ptr<CSGNode>(new CSGNode(type, data));
    }

    [[nodiscard]] static std::unique_ptr<CSGNode> create(
        const Type type,
        CSGNodeData* data,
        std::unique_ptr<CSGNode>&& lhs
    ) {
        auto result = create(type, data);
        result->lhs(std::move(lhs));
        return result;
    }
    
    [[nodiscard]] static std::unique_ptr<CSGNode> create(
        const Type type,
        CSGNodeData* data,
        std::unique_ptr<CSGNode>&& lhs,
        std::unique_ptr<CSGNode>&& rhs 
    ) {
        auto result = create(type, data, std::move(lhs));
        result->rhs(std::move(rhs));
        return result;
    }

    static CSGNode create_empty() {
        CSGNode empty(Type::EMPTY, nullptr);
        return empty;
    }

    const Type type;

    iterator begin() const;
    iterator end() const;

    const CSGNodeData* data() const;

    template <class T>
    const T& data() const { return dynamic_cast<T&>(data()); }

    const CSGNode& parent() const;

    const CSGNode lhs() const;
    void lhs(std::unique_ptr<CSGNode>&& other);

    const CSGNode rhs() const;
    void rhs(std::unique_ptr<CSGNode>&& other);

private:
    // FIXME: Use get/set to manage parents and the like.
    CSGNodeData* _data;
    CSGNode* _parent;
    std::unique_ptr<CSGNode> _lhs;
    std::unique_ptr<CSGNode> _rhs;

};

} // namespace csg
} // namespace imk