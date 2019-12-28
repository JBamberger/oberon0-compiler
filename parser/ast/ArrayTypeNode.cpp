#include "ArrayTypeNode.h"
#include "NodeVisitor.h"
#include <cassert>

ArrayTypeNode::ArrayTypeNode(const FilePos& pos, const int size, std::shared_ptr<TypeNode> type)
    : TypeNode(NodeType::array_type, pos), size_(size), type_(std::move(type))
{
    assert(type_ != nullptr);
}

ArrayTypeNode::~ArrayTypeNode() = default;

int ArrayTypeNode::getSize() const { return size_; }

std::shared_ptr<TypeNode> ArrayTypeNode::getType() const { return type_; }

void ArrayTypeNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void ArrayTypeNode::print(std::ostream& stream) const
{
    stream << "ArrayTypeNode(" << size_ << "," << *type_ << ")";
}
