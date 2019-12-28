#include "FieldDeclarationNode.h"

#include "NodeVisitor.h"
#include <utility>

FieldDeclarationNode::FieldDeclarationNode(const FilePos& pos, std::string name, std::string type)
    : TypedIdentifierNode(NodeType::field, pos, std::move(name), std::move(type))
{
}

FieldDeclarationNode::~FieldDeclarationNode() = default;

void FieldDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void FieldDeclarationNode::print(std::ostream& stream) const
{
    stream << "FieldNode(" << name_ << ", " << type_ << ")";
}
