#pragma once
#include "ActualParameterNode.h"
#include "StatementNode.h"
#include <memory>
#include <vector>

class ProcedureCallNode : public StatementNode {
    std::string name_;
    std::unique_ptr<std::vector<std::unique_ptr<ActualParameterNode>>> parameters_;

  public:
    ProcedureCallNode(const FilePos& pos, std::string name);
    ~ProcedureCallNode() override;

    const std::string& getName() const;
    const std::unique_ptr<std::vector<std::unique_ptr<ActualParameterNode>>>& getParameters() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
