#pragma once
#include "ActualParameterNode.h"
#include "StatementNode.h"
#include "VariableReferenceNode.h"

class ProcedureCallNode : public StatementNode {
    std::unique_ptr<const VariableReferenceNode> name_;
    std::unique_ptr<const ActualParameterNode> parameters_;

  public:
    ProcedureCallNode(const VariableReferenceNode* name, const ActualParameterNode* parameters);
    ~ProcedureCallNode() override;
    void print(std::ostream& stream) const override;
};
