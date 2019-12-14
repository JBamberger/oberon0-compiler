#include "StringConstantNode.h"

StringConstantNode::StringConstantNode(const FilePos& pos, std::string value)
    : ExpressionNode(NodeType::string_constant, pos), value_(value)
{
}

const std::string& StringConstantNode::getValue() const { return value_; }

void StringConstantNode::print(std::ostream& stream) const
{
    stream << "StringConstant(" << value_ << ")";
}
