#include "StringConstantNode.h"
#include "NodeVisitor.h"
#include "Scope.h"

StringConstantNode::StringConstantNode(const FilePos& pos, std::string value)
    : ConstantNode(NodeType::string_constant, pos, "STRING"), value_(std::move(value))
{
}

const std::string& StringConstantNode::getValue() const { return value_; }
void StringConstantNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void StringConstantNode::print(std::ostream& stream) const
{
    stream << "StringConstant(" << value_ << ")";
}
