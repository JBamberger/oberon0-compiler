#pragma once
#include "ExpressionNode.h"

class FieldReferenceNode : public AssignableExpressionNode {

    std::unique_ptr<AssignableExpressionNode> record_ref_;
    std::string field_name_;

  public:
    FieldReferenceNode(const FilePos& pos,
                       std::string field_name,
                       std::shared_ptr<TypeNode> type,
                       std::unique_ptr<AssignableExpressionNode> record_ref);
    ~FieldReferenceNode() override;

    const std::string& getFieldName() const;
    const std::unique_ptr<AssignableExpressionNode>& getRecordRef() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
