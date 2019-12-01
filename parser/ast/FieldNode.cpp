#include "FieldNode.h"
#include <cassert>
#include <utility>

FieldNode::FieldNode(const FilePos& pos, std::string name, const TypeReferenceNode* type)
    : TypedIdentifierNode(NodeType::field, pos, std::move(name), type)
{
    assert(type_ != nullptr);
}

FieldNode::~FieldNode() = default;

void FieldNode::print(std::ostream& stream) const
{
    stream << "FieldNode(" << name_ << ", " << *type_ << ")";
}
