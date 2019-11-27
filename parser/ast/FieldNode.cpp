#include "FieldNode.h"

FieldNode::FieldNode(const FilePos& pos, std::string name, const TypeReferenceNode* type)
    : Node(NodeType::field, pos), name_(std::move(name)), type_(type)
{
}

FieldNode::~FieldNode() = default;

void FieldNode::print(std::ostream& stream) const
{
    stream << "FieldNode(" << name_ << ", " << type_ << ")";
}
