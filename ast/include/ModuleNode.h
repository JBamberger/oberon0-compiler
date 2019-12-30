#pragma once
#include "BlockNode.h"
#include "ProcedureDeclarationNode.h"

class ModuleNode : public BlockNode {
    ProcDeclList procedures_;

  public:
    ModuleNode(const FilePos& pos, const std::string& name);
    ~ModuleNode() override;
    ProcDeclList& getProcedures() override;
    const ProcDeclList& getProcedures() const override;
    void print(std::ostream& stream) const override;
    void visit(NodeVisitor* visitor) const override;
};
