#include "FieldDeclarationNode.h"

#include "NodeVisitor.h"
#include <cassert>
#include <utility>

FieldDeclarationNode::FieldDeclarationNode(const FilePos& pos,
                                           std::string name,
                                           std::shared_ptr<TypeNode> type)
    : TypedIdentifierNode(NodeType::field, pos, std::move(name), std::move(type))
{
    assert(type_ != nullptr);
}

FieldDeclarationNode::~FieldDeclarationNode() = default;

void FieldDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void FieldDeclarationNode::print(std::ostream& stream) const
{
    stream << "FieldNode(" << name_ << ", " << *type_ << ")";
}
