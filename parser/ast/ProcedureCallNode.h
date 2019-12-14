#pragma once
#include "ActualParameterNode.h"
#include "StatementNode.h"
#include "VariableReferenceNode.h"

class ProcedureCallNode : public StatementNode {
    std::string name_;
    std::unique_ptr<const ActualParameterNode> parameters_;

  public:
    ProcedureCallNode(const FilePos& pos, std::string name, const ActualParameterNode* parameters);
    ~ProcedureCallNode() override;
    void print(std::ostream& stream) const override;
};
