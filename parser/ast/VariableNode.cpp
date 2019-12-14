#include "VariableNode.h"
#include "NodeVisitor.h"
#include <utility>

VariableNode::VariableNode(const FilePos& pos, std::string name, const TypeReferenceNode* type)
    : TypedIdentifierNode(NodeType::variable, pos, std::move(name), type)
{
    assert(type_ != nullptr);
}

VariableNode::~VariableNode() = default;

void VariableNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void VariableNode::print(std::ostream& stream) const
{
    stream << "VariableNode(" << name_ << ", " << *type_ << ")";
}
