#pragma once
#include "ExpressionNode.h"
#include "FieldDeclarationNode.h"

class FieldReferenceNode : public AssignableExpressionNode {

    std::unique_ptr<AssignableExpressionNode> record_ref_;
    FieldDeclarationNode* field_;

  public:
    FieldReferenceNode(const FilePos& pos,
                       FieldDeclarationNode* field,
                       std::unique_ptr<AssignableExpressionNode> record_ref);
    ~FieldReferenceNode() override;

    std::string getFieldName() const;
    FieldDeclarationNode* getField() const;
    const std::unique_ptr<AssignableExpressionNode>& getRecordRef() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
