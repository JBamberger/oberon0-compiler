#include "VariableNode.h"
#include <utility>

VariableNode::VariableNode(const FilePos& pos, std::string name, const TypeReferenceNode* type)
    : TypedIdentifierNode(NodeType::variable, pos, std::move(name), type)
{
    assert(type_ != nullptr);
}

VariableNode::~VariableNode() = default;

void VariableNode::print(std::ostream& stream) const
{
    stream << "VariableNode(" << name_ << ", " << *type_ << ")";
}
