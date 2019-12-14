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

    const std::string& getName() const;
    const std::unique_ptr<const ActualParameterNode>& getParameters() const;
    void print(std::ostream& stream) const override;
};
