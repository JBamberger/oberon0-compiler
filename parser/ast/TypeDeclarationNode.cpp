#include "TypeDeclarationNode.h"

TypeDeclarationNode::TypeDeclarationNode(const FilePos& pos, std::string name, const TypeNode* type)
    : Node(NodeType::type_reference, pos), name_(name), type_(type)
{
}

TypeDeclarationNode::~TypeDeclarationNode() = default;

void TypeDeclarationNode::print(std::ostream& stream) const
{
    stream << "TypeDeclarationNode(" << name_ << ", " << type_ << ")";
}
