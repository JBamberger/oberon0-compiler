#pragma once
#include "TypeReferenceNode.h"
#include "TypedIdentifierNode.h"

class FieldDeclarationNode : public TypedIdentifierNode {
  public:
    FieldDeclarationNode(const FilePos& pos,
                         std::string name,
                         std::unique_ptr<TypeReferenceNode> type);

    ~FieldDeclarationNode() override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
