#include "ArrayReferenceNode.h"
#include "NodeVisitor.h"
#include <cassert>
#include <utility>

ArrayReferenceNode::ArrayReferenceNode(const FilePos& pos, std::unique_ptr<ExpressionNode> index)
    : SelectorNode(pos), index_(std::move(index))
{
    assert(index_ != nullptr);
}

ArrayReferenceNode::~ArrayReferenceNode() = default;

const std::unique_ptr<ExpressionNode>& ArrayReferenceNode::getIndex() const { return index_; }
void ArrayReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ArrayReferenceNode::print(std::ostream& stream) const
{
    stream << "ArrayReferenceNode(" << *index_;
    if (next_ != nullptr) {
        stream << ", " << *next_;
    }
    stream << ")";
}
