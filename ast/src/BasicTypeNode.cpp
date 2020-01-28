#include "BasicTypeNode.h"
#include "NodeVisitor.h"

BasicTypeNode::BasicTypeNode(std::string name, const size_t size)
    : TypeNode(), name_(std::move(name)), size_(size)
{
}

BasicTypeNode::~BasicTypeNode() = default;

const std::string& BasicTypeNode::getName() const { return name_; }

std::string BasicTypeNode::getId() const { return name_; }

void BasicTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void BasicTypeNode::print(std::ostream& stream) const { stream << "BasicType(" << name_ << ")"; }

std::unique_ptr<BasicTypeNode> BasicTypeNode::makeInt()
{
    return std::make_unique<BasicTypeNode>("INTEGER", 8);
}

std::unique_ptr<BasicTypeNode> BasicTypeNode::makeString()
{
    return std::make_unique<BasicTypeNode>("STRING", 8); // just a pointer
}

std::unique_ptr<BasicTypeNode> BasicTypeNode::makeBool()
{
    return std::make_unique<BasicTypeNode>("BOOLEAN", 8);
}

size_t BasicTypeNode::getByteSize() const {
    return size_;
}
