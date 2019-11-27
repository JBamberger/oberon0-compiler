#include "BasicTypeNode.h"

BasicTypeNode::BasicTypeNode(const FilePos& pos, std::string name)
    : TypeNode(NodeType::basic_type, pos), name_(std::move(name))
{
}

BasicTypeNode::~BasicTypeNode() = default;

void BasicTypeNode::print(std::ostream& stream) const { stream << "BasicType(" << name_ << ")"; }