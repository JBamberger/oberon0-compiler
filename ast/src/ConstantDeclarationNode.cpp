#include "ConstantDeclarationNode.h"
#include "NodeVisitor.h"
#include <cassert>

ConstantDeclarationNode::ConstantDeclarationNode(const FilePos& pos,
                                                 std::string name,
                                                 std::unique_ptr<ConstantNode> value)
    : Node(NodeType::constant_declaration, pos), name_(std::move(name)), value_(std::move(value))
{
    assert(value_ != nullptr);
}

ConstantDeclarationNode::~ConstantDeclarationNode() = default;

const std::string& ConstantDeclarationNode::getName() const
{
    return name_;
}

const std::unique_ptr<ConstantNode>& ConstantDeclarationNode::getValue() const
{
    return value_;
}

void ConstantDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ConstantDeclarationNode::print(std::ostream& stream) const
{
    stream << "ConstantDeclarationNode(" << name_ << ", " << *value_ << ")";
}
