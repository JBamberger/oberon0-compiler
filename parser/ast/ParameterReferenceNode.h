#pragma once
#include "ExpressionNode.h"
#include "ParameterDeclarationNode.h"

class ParameterReferenceNode : public AssignableExpressionNode {

    ParameterDeclarationNode* param_;

  public:
    ParameterReferenceNode(const FilePos& pos, ParameterDeclarationNode* param);
    ~ParameterReferenceNode() override;

    std::string getName() const;

    ParameterDeclarationNode* getParam() const;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
