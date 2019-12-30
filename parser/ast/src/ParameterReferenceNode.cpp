#include "ParameterReferenceNode.h"

#include "NodeVisitor.h"
#include <utility>

ParameterReferenceNode::ParameterReferenceNode(const FilePos& pos, ParameterDeclarationNode* param)
    : AssignableExpressionNode(NodeType::variable_reference, pos, param->getType()), param_(param)
{
}

ParameterReferenceNode::~ParameterReferenceNode() = default;

std::string ParameterReferenceNode::getName() const { return param_->getName(); }

ParameterDeclarationNode* ParameterReferenceNode::getParam() const { return param_; }

void ParameterReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ParameterReferenceNode::print(std::ostream& stream) const
{
    stream << "ParameterReferenceNode(" << param_->getType() << ")";
}
