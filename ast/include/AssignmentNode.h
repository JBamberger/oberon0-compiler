#pragma once
#include "StatementNode.h"
#include "VariableReferenceNode.h"

class AssignmentNode : public StatementNode {
    std::unique_ptr<AssignableExpressionNode> assignee_;
    std::unique_ptr<ExpressionNode> value_;

  public:
    AssignmentNode(const FilePos& pos,
                   std::unique_ptr<AssignableExpressionNode> assignee,
                   std::unique_ptr<ExpressionNode> value);

    ~AssignmentNode() override;

    const std::unique_ptr<AssignableExpressionNode>& getAssignee() const;

    const std::unique_ptr<ExpressionNode>& getValue() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
