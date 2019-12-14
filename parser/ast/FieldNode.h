#pragma once
#include "TypeReferenceNode.h"
#include "TypedIdentifierNode.h"

class FieldNode : public TypedIdentifierNode {
  public:
    FieldNode(const FilePos& pos, std::string name, const TypeReferenceNode* type);

    ~FieldNode() override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
