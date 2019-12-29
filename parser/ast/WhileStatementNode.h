#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include <vector>

class WhileStatementNode : public StatementNode {
  public:
    using StmtList = std::vector<std::unique_ptr<StatementNode>>;

  private:
    std::unique_ptr<ExpressionNode> condition_;
    StmtList body_;

  public:
    WhileStatementNode(const FilePos& pos, std::unique_ptr<ExpressionNode> condition);
    ~WhileStatementNode() override;

    const std::unique_ptr<ExpressionNode>& getCondition() const;
    StmtList& getBody();
    const StmtList& getBody() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
