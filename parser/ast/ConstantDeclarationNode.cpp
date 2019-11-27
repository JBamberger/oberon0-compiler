#include "ConstantDeclarationNode.h"

ConstantDeclarationNode::ConstantDeclarationNode(const FilePos& pos,
                                                 std::string name,
                                                 const ExpressionNode* value)
    : Node(NodeType::constant_declaration, pos), name_(std::move(name)), value_(value)
{
}

ConstantDeclarationNode::~ConstantDeclarationNode() = default;

void ConstantDeclarationNode::print(std::ostream& stream) const
{
    stream << "ConstantDeclarationNode(" << name_ << ", " << value_ << ")";
}
