#include "BasicTypeNode.h"
#include "NodeVisitor.h"

BasicTypeNode::BasicTypeNode(const FilePos& pos, std::string name)
    : TypeNode(NodeType::basic_type, pos), name_(std::move(name))
{
}

BasicTypeNode::~BasicTypeNode() = default;

const std::string& BasicTypeNode::getName() const { return name_; }
void BasicTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void BasicTypeNode::print(std::ostream& stream) const { stream << "BasicType(" << name_ << ")"; }
