#include "FieldNode.h"
#include "NodeVisitor.h"
#include <cassert>
#include <utility>

FieldNode::FieldNode(const FilePos& pos, std::string name, const TypeReferenceNode* type)
    : TypedIdentifierNode(NodeType::field, pos, std::move(name), type)
{
    assert(type_ != nullptr);
}

FieldNode::~FieldNode() = default;

void FieldNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void FieldNode::print(std::ostream& stream) const
{
    stream << "FieldNode(" << name_ << ", " << *type_ << ")";
}
