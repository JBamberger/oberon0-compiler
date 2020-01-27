#pragma once
#include "TypedIdentifierNode.h"

class VariableDeclarationNode : public TypedIdentifierNode {
  public:
    VariableDeclarationNode(const FilePos& pos, std::string name, std::string type);

    ~VariableDeclarationNode() override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
