#include "FieldReferenceNode.h"
#include "NodeVisitor.h"
#include <utility>

FieldReferenceNode::FieldReferenceNode(const FilePos& pos,
                                       FieldDeclarationNode* field,
                                       std::unique_ptr<AssignableExpressionNode> record_ref)
    : AssignableExpressionNode(NodeType::selector, pos, field->getType()),
      record_ref_(std::move(record_ref)), field_(field)
{
}

const std::unique_ptr<AssignableExpressionNode>& FieldReferenceNode::getRecordRef() const
{
    return record_ref_;
}

FieldReferenceNode::~FieldReferenceNode() = default;

std::string FieldReferenceNode::getFieldName() const
{
    return field_->getName();
}

FieldDeclarationNode* FieldReferenceNode::getField() const
{
    return field_;
}

void FieldReferenceNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void FieldReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << *record_ref_ << ", " << field_->getName() << ")";
}
