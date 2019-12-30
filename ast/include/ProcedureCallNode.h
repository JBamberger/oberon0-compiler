#pragma once
#include "ExpressionNode.h"
#include "ProcedureDeclarationNode.h"
#include "StatementNode.h"
#include <memory>
#include <vector>

using ActualParamList = std::vector<std::unique_ptr<ExpressionNode>>;

class ProcedureCallNode : public StatementNode {
    ProcedureDeclarationNode* procedure_;
    ActualParamList params_;

  public:
    ProcedureCallNode(const FilePos& pos,
                      ProcedureDeclarationNode* procedure,
                      ActualParamList params);
    ~ProcedureCallNode() override;

    const std::string& getName() const;
    ProcedureDeclarationNode* getProcedure() const;
    ActualParamList& getParams();
    const ActualParamList& getParams() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
