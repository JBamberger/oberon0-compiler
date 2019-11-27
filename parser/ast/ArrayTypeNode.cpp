#include "ArrayTypeNode.h"

ArrayTypeNode::ArrayTypeNode(const FilePos& pos, const ExpressionNode* value, const TypeNode* type)
    : TypeNode(NodeType::array_type, pos), value_(value), type_(type)
{
}

ArrayTypeNode::~ArrayTypeNode() = default;

void ArrayTypeNode::print(std::ostream& stream) const
{
    stream << "ArrayTypeNode(" << *value_ << "," << *type_ << ")";
}