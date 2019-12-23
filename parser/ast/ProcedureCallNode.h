#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include <memory>
#include <vector>

class ProcedureCallNode : public StatementNode {
    std::string name_;
    std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> parameters_;

  public:
    ProcedureCallNode(const FilePos& pos, std::string name);
    ~ProcedureCallNode() override;

    const std::string& getName() const;
    const std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>>& getParameters() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
