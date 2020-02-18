#include "StringConstantNode.h"
#include "NodeVisitor.h"
#include "Scope.h"
#include <cassert>

StringConstantNode::StringConstantNode(const FilePos& pos, std::string value, TypeNode* type)
    : ConstantNode(pos, type), value_(std::move(value))
{
    assert(type->getId() == "STRING");
}

const std::string& StringConstantNode::getValue() const
{
    return value_;
}

void StringConstantNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void StringConstantNode::print(std::ostream& stream) const
{
    stream << "StringConstant(" << value_ << ")";
}
