#pragma once
#include "TypedIdentifierNode.h"

class FieldDeclarationNode : public TypedIdentifierNode {
  public:
    FieldDeclarationNode(const FilePos& pos, std::string name, TypeNode* type);

    ~FieldDeclarationNode() override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
