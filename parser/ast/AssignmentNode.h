#pragma once
#include "StatementNode.h"
#include "VariableReferenceNode.h"

class AssignmentNode : public StatementNode {
    std::unique_ptr<const VariableReferenceNode> assignee_;
    std::unique_ptr<const ExpressionNode> value_;

  public:
    AssignmentNode(const FilePos& pos,
                   const VariableReferenceNode* assignee,
                   const ExpressionNode* value);
    ~AssignmentNode() override;
    void print(std::ostream& stream) const override;
};