#pragma once
#include "ExpressionNode.h"

class VariableReferenceNode : public AssignableExpressionNode {

    std::string name_;

  public:
    VariableReferenceNode(const FilePos& pos, std::string name, std::shared_ptr<TypeNode> type);
    ~VariableReferenceNode() override;

    const std::string& getName() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
