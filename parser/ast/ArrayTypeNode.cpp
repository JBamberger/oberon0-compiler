#include "ArrayTypeNode.h"
#include <cassert>

ArrayTypeNode::ArrayTypeNode(const FilePos& pos, const ExpressionNode* value, const TypeNode* type)
    : TypeNode(NodeType::array_type, pos), value_(value), type_(type)
{
    assert(value_ != nullptr);
    assert(type_ != nullptr);
}

ArrayTypeNode::~ArrayTypeNode() = default;

const std::unique_ptr<const ExpressionNode>& ArrayTypeNode::getValue() const { return value_; }

const std::unique_ptr<const TypeNode>& ArrayTypeNode::getType() const { return type_; }

void ArrayTypeNode::print(std::ostream& stream) const
{
    stream << "ArrayTypeNode(" << *value_ << "," << *type_ << ")";
}
