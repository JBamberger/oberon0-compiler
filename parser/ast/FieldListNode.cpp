#include "FieldListNode.h"

FieldListNode::FieldListNode(const FilePos& pos,
                             const IdentifierListNode* names,
                             const TypeNode* type)
    : Node(NodeType::field_list, pos), names_(names), type_(type)
{
}

FieldListNode::~FieldListNode() = default;

std::shared_ptr<const TypeNode> FieldListNode::getType() const { return type_; }

const IdentifierListNode* FieldListNode::getNames() const { return names_.get(); }

void FieldListNode::print(std::ostream& stream) const
{
    stream << "FieldListNode(" << names_ << ", " << type_ << ")";
}
