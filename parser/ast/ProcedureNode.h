#pragma once
#include "BlockNode.h"

using ParamList = std::vector<std::unique_ptr<const ParameterListNode>>;

class ProcedureNode : public BlockNode {
    std::unique_ptr<ParamList> params_;
    std::unique_ptr<std::vector<std::unique_ptr<ProcedureNode>>> procedures_;

  public:
    ProcedureNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> parent);
    ~ProcedureNode() override;

    const std::unique_ptr<ParamList>& getParams() const;
    const std::unique_ptr<std::vector<std::unique_ptr<ProcedureNode>>>&
    getProcedures() const override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
