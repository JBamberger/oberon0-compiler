#pragma once
#include "TypeReferenceNode.h"
#include "TypedIdentifierNode.h"
#include <cassert>

class VariableDeclarationNode : public TypedIdentifierNode {
  public:
    VariableDeclarationNode(const FilePos& pos,
                            std::string name,
                            std::unique_ptr<TypeReferenceNode> type);

    ~VariableDeclarationNode() override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
