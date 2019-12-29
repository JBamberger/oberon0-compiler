#pragma once
#include "BlockNode.h"
#include "ParameterDeclarationNode.h"

class ProcedureDeclarationNode : public BlockNode {
  public:
    using ParamDeclList = std::vector<std::unique_ptr<ParameterDeclarationNode>>;

  private:
    ParamDeclList params_;
    ProcDeclList procedures_;

  public:
    ProcedureDeclarationNode(const FilePos& pos,
                             const std::string& name,
                             const std::shared_ptr<Scope>& parent);
    ~ProcedureDeclarationNode() override;

    ParamDeclList& getParams();
    const ParamDeclList& getParams() const;
    ProcDeclList& getProcedures() override;
    const ProcDeclList& getProcedures() const override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
