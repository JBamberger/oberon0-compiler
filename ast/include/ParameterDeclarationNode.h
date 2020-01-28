#pragma once
#include "TypedIdentifierNode.h"

class ParameterDeclarationNode : public TypedIdentifierNode {
    bool is_reference_;

  public:
    ParameterDeclarationNode(const FilePos& pos,
                             std::string name,
                             TypeNode* type,
                             bool is_reference);

    ~ParameterDeclarationNode() override;

    bool isIsReference() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
