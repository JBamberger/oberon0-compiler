#include "VariableReferenceNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableReferenceNode::VariableReferenceNode(const FilePos& pos, VariableDeclarationNode* variable)
    : AssignableExpressionNode(NodeType::variable_reference, pos, variable->getType()),
      variable_(variable)
{
}

VariableReferenceNode::~VariableReferenceNode() = default;

std::string VariableReferenceNode::getName() const { return variable_->getName(); }

VariableDeclarationNode* VariableReferenceNode::getVariable() const { return variable_; }

void VariableReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void VariableReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << variable_->getType() << ")";
}
