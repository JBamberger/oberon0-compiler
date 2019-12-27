#include "FieldReferenceNode.h"
#include "NodeVisitor.h"
#include <utility>

FieldReferenceNode::FieldReferenceNode(const FilePos& pos,
                                       std::string field_name,
                                       std::shared_ptr<TypeNode> type,
                                       std::unique_ptr<AssignableExpressionNode> record_ref)
    : AssignableExpressionNode(NodeType::selector, pos, std::move(type)),
      record_ref_(std::move(record_ref)), field_name_(std::move(field_name))
{
}

const std::unique_ptr<AssignableExpressionNode>& FieldReferenceNode::getRecordRef() const
{
    return record_ref_;
}

FieldReferenceNode::~FieldReferenceNode() = default;

const std::string& FieldReferenceNode::getFieldName() const { return field_name_; }

void FieldReferenceNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void FieldReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << record_ref_ << ", " << field_name_ << ")";
}
