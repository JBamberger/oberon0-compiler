#pragma once
#include "TypeReferenceNode.h"
#include "TypedIdentifierNode.h"

class FieldNode : public TypedIdentifierNode {
  public:
    FieldNode(const FilePos& pos, std::string name, const TypeReferenceNode* type);

    ~FieldNode() override;

    void print(std::ostream& stream) const override;
};
