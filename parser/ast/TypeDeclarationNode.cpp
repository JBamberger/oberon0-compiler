#include "TypeDeclarationNode.h"
#include <cassert>
#include <utility>

TypeDeclarationNode::TypeDeclarationNode(const FilePos& pos, std::string name, const TypeNode* type)
    : Node(NodeType::type_reference, pos), name_(std::move(name)), type_(type)
{
    assert(type_ != nullptr);
}

TypeDeclarationNode::~TypeDeclarationNode() = default;

void TypeDeclarationNode::print(std::ostream& stream) const
{
    stream << "TypeDeclarationNode(" << name_ << ", " << *type_ << ")";
}
