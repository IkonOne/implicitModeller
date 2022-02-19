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

CSGNode::VisitorIterator CSGNode::VisitorIterator::operator++(int) {
    VisitorIterator tmp = *this;
    ++(*this);
    return tmp;
}

CSGNode::iterator CSGNode::begin() const { return iterator(const_cast<CSGNode*>(this), const_cast<CSGNode*>(this)); }
CSGNode::iterator CSGNode::end() const { return iterator(nullptr, const_cast<CSGNode*>(this)); }

const CSGNodeData& CSGNode::data() const { return *this->_data; }
std::shared_ptr<CSGNode> CSGNode::parent() const { return this->_parent; }

std::shared_ptr<CSGNode> CSGNode::lhs() const { return this->_lhs; }
void CSGNode::lhs(const std::shared_ptr<CSGNode>& other) {
    other->_parent = this->shared_from_this();
    this->_lhs = other;
}

std::shared_ptr<CSGNode> CSGNode::rhs() const { return this->_rhs; }
void CSGNode::rhs(const std::shared_ptr<CSGNode>& other) {
    other->_parent = this->shared_from_this();
    this->_rhs = other;
}

} // namespace csg
} // namespace imk