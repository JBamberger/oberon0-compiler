#include "ArrayReferenceNode.h"
#include "NodeVisitor.h"
#include <cassert>
#include <utility>

ArrayReferenceNode::ArrayReferenceNode(const FilePos& pos,
                                       std::unique_ptr<ExpressionNode> index,
                                       std::shared_ptr<TypeNode> type,
                                       std::unique_ptr<AssignableExpressionNode> array_ref)
    : AssignableExpressionNode(NodeType::selector, pos, std::move(type)),
      array_ref_(std::move(array_ref)), index_(std::move(index))
{
    assert(array_ref_ != nullptr);
    assert(index_ != nullptr);
}

ArrayReferenceNode::~ArrayReferenceNode() = default;

const std::unique_ptr<AssignableExpressionNode>& ArrayReferenceNode::getArrayRef() const
{
    return array_ref_;
}

const std::unique_ptr<ExpressionNode>& ArrayReferenceNode::getIndex() const { return index_; }

void ArrayReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ArrayReferenceNode::print(std::ostream& stream) const
{
    stream << "ArrayReferenceNode(" << *array_ref_ << ", " << *index_ << ")";
}
