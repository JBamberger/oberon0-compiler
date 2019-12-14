#include "ArrayReferenceNode.h"
#include <cassert>
#include <utility>

ArrayReferenceNode::ArrayReferenceNode(const FilePos& pos, const ExpressionNode* index)
    : SelectorNode(pos), index_(index)
{
    assert(index_ != nullptr);
}

ArrayReferenceNode::~ArrayReferenceNode() = default;

const std::unique_ptr<const ExpressionNode>& ArrayReferenceNode::getIndex() const { return index_; }

void ArrayReferenceNode::print(std::ostream& stream) const
{
    stream << "ArrayReferenceNode(" << *index_;
    if (next_ != nullptr) {
        stream << ", " << *next_;
    }
    stream << ")";
}
