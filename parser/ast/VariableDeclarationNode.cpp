#include "VariableDeclarationNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableDeclarationNode::VariableDeclarationNode(const FilePos& pos,
                                                 std::string name,
                                                 std::shared_ptr<TypeNode> type)
    : TypedIdentifierNode(NodeType::variable, pos, std::move(name), std::move(type))
{
    assert(type_ != nullptr);
}

VariableDeclarationNode::~VariableDeclarationNode() = default;

void VariableDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void VariableDeclarationNode::print(std::ostream& stream) const
{
    stream << "VariableNode(" << name_ << ", " << *type_ << ")";
}
