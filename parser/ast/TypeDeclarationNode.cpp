#include "TypeDeclarationNode.h"
#include "NodeVisitor.h"
#include <cassert>
#include <utility>

TypeDeclarationNode::TypeDeclarationNode(const FilePos& pos,
                                         std::string name,
                                         std::shared_ptr<TypeNode> type)
    : Node(NodeType::type_reference, pos), name_(std::move(name)), type_(std::move(type))
{
    assert(type_ != nullptr);
}

TypeDeclarationNode::~TypeDeclarationNode() = default;

const std::string& TypeDeclarationNode::getName() const { return name_; }

const std::shared_ptr<TypeNode>& TypeDeclarationNode::getType() const { return type_; }

void TypeDeclarationNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void TypeDeclarationNode::print(std::ostream& stream) const
{
    stream << "TypeDeclarationNode(" << name_ << ", " << *type_ << ")";
}
