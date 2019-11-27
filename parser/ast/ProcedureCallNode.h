#pragma once
#include "ActualParameterNode.h"
#include "MemberReferenceNode.h"
#include "StatementNode.h"

class ProcedureCallNode : public StatementNode {
    std::unique_ptr<const MemberReferenceNode> name_;
    std::unique_ptr<const ActualParameterNode> parameters_;

  public:
    ProcedureCallNode(const MemberReferenceNode* name, const ActualParameterNode* parameters);
    ~ProcedureCallNode() override;
    void print(std::ostream& stream) const override;
};
