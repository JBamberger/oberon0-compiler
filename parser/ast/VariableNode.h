#pragma once
#include "TypeReferenceNode.h"
#include "TypedIdentifierNode.h"
#include <cassert>

class VariableNode : public TypedIdentifierNode {
  public:
    VariableNode(const FilePos& pos, std::string name, const TypeReferenceNode* type);

    ~VariableNode() override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
