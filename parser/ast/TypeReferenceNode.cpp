#include "TypeReferenceNode.h"
#include <utility>
#include <cassert>

TypeReferenceNode::TypeReferenceNode(const FilePos& pos, std::shared_ptr<const TypeNode> type)
    : Node(NodeType::type_reference, pos), type_(std::move(type))
{
    assert(type_ != nullptr);
}

TypeReferenceNode::~TypeReferenceNode() = default;

const std::shared_ptr<const TypeNode>& TypeReferenceNode::getType() const { return type_; }

void TypeReferenceNode::print(std::ostream& stream) const
{
    stream << "TypeReferenceNode(" << *type_ << ")";
}
