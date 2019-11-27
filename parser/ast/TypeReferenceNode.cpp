#include "TypeReferenceNode.h"
#include <utility>

TypeReferenceNode::TypeReferenceNode(const FilePos& pos, std::shared_ptr<const TypeNode> type)
    : Node(NodeType::type_reference, pos), type_(std::move(type))
{
}

TypeReferenceNode::~TypeReferenceNode() = default;

void TypeReferenceNode::print(std::ostream& stream) const
{
    stream << "TypeReferenceNode(" << *type_ << ")";
}
