#include "ArrayTypeNode.h"
#include "NodeVisitor.h"

ArrayTypeNode::ArrayTypeNode(const FilePos& pos, const int size, std::string type)
    : TypeNode(NodeType::array_type, pos), size_(size), type_(std::move(type))
{
}

ArrayTypeNode::~ArrayTypeNode() = default;

int ArrayTypeNode::getSize() const { return size_; }

std::string ArrayTypeNode::getType() const { return type_; }

std::string ArrayTypeNode::getId() const
{
    return "[A," + std::to_string(size_) + "," + type_ + "]";
}

void ArrayTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ArrayTypeNode::print(std::ostream& stream) const
{
    stream << "ArrayTypeNode(" << size_ << "," << type_ << ")";
}