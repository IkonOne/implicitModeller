#include "csg_node.h"

namespace imk {
namespace csg {

const CSGNode::VisitorIterator::VisitorState CSGNode::VisitorIterator::state() const {
    return this->_state;
}

CSGNode::VisitorIterator::reference CSGNode::VisitorIterator::operator*() const { return *_ptr; }
CSGNode::VisitorIterator::pointer CSGNode::VisitorIterator::operator->() const { return _ptr; }


CSGNode::VisitorIterator& CSGNode::VisitorIterator::operator++() {
    switch(_state) {
        case PreOrder:
            if (_ptr->_lhs) {
                _ptr = _ptr->_lhs.get();
            }
            else {
                _state = InOrder;
            }
            break;

        case InOrder:
            if (_ptr->_rhs) {
                _state = PreOrder;
                _ptr = _ptr->_rhs.get();
            }
            else {
                _state = PostOrder;
            }
            break;

        case PostOrder:
            pointer parent = _ptr->_parent;

            if (parent == nullptr || _ptr == _root) {
                _ptr = nullptr;
            }
            else { // parent exists
                if (parent->_lhs.get() == _ptr) {
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

CSGNode::VisitorIterator CSGNode::VisitorIterator::operator++(int) {
    VisitorIterator tmp = *this;
    ++(*this);
    return tmp;
}

CSGNode::CSGNode(const CSGNode& other)
    : type(other.type), _data(other._data)
{
    if (other._lhs) {
        std::unique_ptr<CSGNode> tmp(new CSGNode(*other._lhs));
        lhs(std::move(tmp));
    }

    if (other._rhs) {
        std::unique_ptr<CSGNode> tmp(new CSGNode(*other._rhs));
        rhs(std::move(tmp));
    }
}

CSGNode::iterator CSGNode::begin() const { return iterator(const_cast<CSGNode*>(this), const_cast<CSGNode*>(this)); }
CSGNode::iterator CSGNode::end() const { return iterator(nullptr, const_cast<CSGNode*>(this)); }

const CSGNodeData* CSGNode::data() const { return this->_data; }
const CSGNode& CSGNode::parent() const { return *(this->_parent); }

const CSGNode CSGNode::lhs() const {
    if (this->_lhs) return *(this->_lhs);
    return create_empty();
}
void CSGNode::lhs(std::unique_ptr<CSGNode>&& other) {
    other->_parent = this;
    this->_lhs = std::move(other);
}

const CSGNode CSGNode::rhs() const {
    if (this->_rhs) return *(this->_rhs);
    return create_empty();
}
void CSGNode::rhs(std::unique_ptr<CSGNode>&& other) {
    other->_parent = this;
    this->_rhs = std::move(other);
}

} // namespace csg
} // namespace imk