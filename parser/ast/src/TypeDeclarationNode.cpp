#include "TypeDeclarationNode.h"
#include "NodeVisitor.h"
#include <utility>

TypeDeclarationNode::TypeDeclarationNode(const FilePos& pos, std::string name, std::string type)
    : Node(NodeType::type_reference, pos), name_(std::move(name)), type_(std::move(type))
{
}

TypeDeclarationNode::~TypeDeclarationNode() = default;

std::string TypeDeclarationNode::getName() const { return name_; }

std::string TypeDeclarationNode::getType() const { return type_; }

void TypeDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void TypeDeclarationNode::print(std::ostream& stream) const
{
    stream << "TypeDeclarationNode(" << name_ << ", " << type_ << ")";
}