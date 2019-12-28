#pragma once
#include "ExpressionNode.h"
#include "VariableDeclarationNode.h"

class VariableReferenceNode : public AssignableExpressionNode {

    VariableDeclarationNode* variable_;

  public:
    VariableReferenceNode(const FilePos& pos, VariableDeclarationNode* variable);
    ~VariableReferenceNode() override;

    std::string getName() const;

    VariableDeclarationNode* getVariable() const;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
