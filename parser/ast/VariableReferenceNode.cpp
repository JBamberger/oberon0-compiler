#include "VariableReferenceNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableReferenceNode::VariableReferenceNode(const FilePos& pos,
                                             std::string name,
                                             std::shared_ptr<TypeNode> type)
    : AssignableExpressionNode(NodeType::variable_reference, pos, std::move(type)),
      name_(std::move(name))
{
}

VariableReferenceNode::~VariableReferenceNode() = default;

const std::string& VariableReferenceNode::getName() const { return name_; }

void VariableReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void VariableReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << name_ << ")";
}
