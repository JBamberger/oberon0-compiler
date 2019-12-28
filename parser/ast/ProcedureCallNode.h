#pragma once
#include "ExpressionNode.h"
#include "ProcedureDeclarationNode.h"
#include "StatementNode.h"
#include <memory>
#include <vector>

class ProcedureCallNode : public StatementNode {
    ProcedureDeclarationNode* procedure_;
    std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> params_;

  public:
    ProcedureCallNode(const FilePos& pos,
                      ProcedureDeclarationNode* procedure,
                      std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> params);
    ~ProcedureCallNode() override;

    const std::string& getName() const;
    ProcedureDeclarationNode* getProcedure() const;
    const std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>>& getParams() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
