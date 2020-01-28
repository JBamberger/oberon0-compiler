#include "VariableDeclarationNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableDeclarationNode::VariableDeclarationNode(const FilePos& pos,
                                                 std::string name,
                                                 TypeNode* type)
    : TypedIdentifierNode(NodeType::variable, pos, std::move(name), type)
{
}

VariableDeclarationNode::~VariableDeclarationNode() = default;

void VariableDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void VariableDeclarationNode::print(std::ostream& stream) const
{
    stream << "VariableNode(" << name_ << ", " << type_ << ")";
}
