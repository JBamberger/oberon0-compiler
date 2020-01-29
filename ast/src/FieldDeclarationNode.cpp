#include "FieldDeclarationNode.h"

#include "NodeVisitor.h"
#include <utility>

FieldDeclarationNode::FieldDeclarationNode(const FilePos& pos, std::string name, TypeNode* type)
    : TypedIdentifierNode(NodeType::field, pos, std::move(name), type)
{
}

FieldDeclarationNode::~FieldDeclarationNode() = default;

void FieldDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void FieldDeclarationNode::print(std::ostream& stream) const
{
    stream << "FieldNode(" << name_ << ", " << type_ << ")";
}
