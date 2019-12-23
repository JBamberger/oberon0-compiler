 #pragma once
#include "BlockNode.h"
#include "ProcedureDeclarationNode.h"

class ModuleNode : public BlockNode {
    std::unique_ptr<std::vector<std::unique_ptr<ProcedureDeclarationNode>>> procedures_;

  public:
    ModuleNode(const FilePos& pos, std::string name);
    ~ModuleNode() override;
    const std::unique_ptr<std::vector<std::unique_ptr<ProcedureDeclarationNode>>>&
    getProcedures() const override;
    void print(std::ostream& stream) const override;
    void visit(NodeVisitor* visitor) const override;
};
