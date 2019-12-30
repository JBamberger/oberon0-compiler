#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include <vector>

class IfStatementNode : public StatementNode {
  public:
    using StmtList = std::vector<std::unique_ptr<StatementNode>>;

  private:
    std::unique_ptr<ExpressionNode> condition_;
    StmtList thenPart_;
    StmtList elsePart_;

  public:
    IfStatementNode(const FilePos& pos, std::unique_ptr<ExpressionNode> condition);
    ~IfStatementNode() override;
    const std::unique_ptr<ExpressionNode>& getCondition() const;
    StmtList& getThenPart();
    StmtList& getElsePart();

    const StmtList& getThenPart() const;
    const StmtList& getElsePart() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
