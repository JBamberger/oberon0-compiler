#pragma once
#include "TypedIdentifierNode.h"

class BlockNode;
class VariableDeclarationNode : public TypedIdentifierNode {
    BlockNode* parent_;

  public:
    VariableDeclarationNode(const FilePos& pos,
                            std::string name,
                            TypeNode* type,
                            BlockNode* parent);

    ~VariableDeclarationNode() override;

    BlockNode* getParent() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
