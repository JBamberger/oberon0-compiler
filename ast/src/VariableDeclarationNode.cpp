#include "VariableDeclarationNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableDeclarationNode::VariableDeclarationNode(const FilePos& pos,
                                                 std::string name,
                                                 TypeNode* type,
                                                 BlockNode* parent)
    : TypedIdentifierNode(NodeType::variable, pos, std::move(name), type), parent_(parent)
{
}

VariableDeclarationNode::~VariableDeclarationNode() = default;

void VariableDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void VariableDeclarationNode::print(std::ostream& stream) const
{
    stream << "VariableNode(" << name_ << ", " << type_ << ")";
}
BlockNode* VariableDeclarationNode::getParent() const
{
    return parent_;
}
