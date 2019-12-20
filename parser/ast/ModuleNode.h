#pragma once
#include "BlockNode.h"
#include "ProcedureNode.h"

class ModuleNode : public BlockNode {
    std::unique_ptr<std::vector<std::unique_ptr<ProcedureNode>>> procedures_;

  public:
    ModuleNode(const FilePos& pos, std::string name);
    ~ModuleNode() override;
    const std::unique_ptr<std::vector<std::unique_ptr<ProcedureNode>>>&
    getProcedures() const override;
    void print(std::ostream& stream) const override;
    void visit(NodeVisitor* visitor) const override;
};
