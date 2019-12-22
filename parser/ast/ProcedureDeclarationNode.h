#pragma once
#include "BlockNode.h"

using ParamList = std::vector<std::unique_ptr<const ParameterDeclarationNode>>;

class ProcedureDeclarationNode : public BlockNode {
    std::unique_ptr<ParamList> params_;
    std::unique_ptr<std::vector<std::unique_ptr<ProcedureDeclarationNode>>> procedures_;

  public:
    ProcedureDeclarationNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> parent);
    ~ProcedureDeclarationNode() override;

    const std::unique_ptr<ParamList>& getParams() const;
    const std::unique_ptr<std::vector<std::unique_ptr<ProcedureDeclarationNode>>>&
    getProcedures() const override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
