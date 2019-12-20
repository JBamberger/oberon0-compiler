#include "ArrayTypeNode.h"
#include "NodeVisitor.h"
#include <cassert>

ArrayTypeNode::ArrayTypeNode(const FilePos& pos,
                             std::unique_ptr<ExpressionNode> value,
                             std::unique_ptr<TypeNode> type)
    : TypeNode(NodeType::array_type, pos), value_(std::move(value)), type_(std::move(type))
{
    assert(value_ != nullptr);
    assert(type_ != nullptr);
}

ArrayTypeNode::~ArrayTypeNode() = default;

const std::unique_ptr<ExpressionNode>& ArrayTypeNode::getValue() const { return value_; }

const std::unique_ptr<TypeNode>& ArrayTypeNode::getType() const { return type_; }

void ArrayTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ArrayTypeNode::print(std::ostream& stream) const
{
    stream << "ArrayTypeNode(" << *value_ << "," << *type_ << ")";
}
