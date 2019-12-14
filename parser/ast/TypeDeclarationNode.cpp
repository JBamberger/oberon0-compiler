#include "TypeDeclarationNode.h"
#include <cassert>
#include <utility>
#include "NodeVisitor.h"

TypeDeclarationNode::TypeDeclarationNode(const FilePos& pos, std::string name, const TypeNode* type)
    : Node(NodeType::type_reference, pos), name_(std::move(name)), type_(type)
{
    assert(type_ != nullptr);
}

TypeDeclarationNode::~TypeDeclarationNode() = default;

const std::string& TypeDeclarationNode::getName() const { return name_; }

const std::shared_ptr<const TypeNode>& TypeDeclarationNode::getType() const { return type_; }
void TypeDeclarationNode::visit(NodeVisitor* visitor) const
{ visitor->visit(this); }

void TypeDeclarationNode::print(std::ostream& stream) const
{
    stream << "TypeDeclarationNode(" << name_ << ", " << *type_ << ")";
}
