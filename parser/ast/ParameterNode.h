#pragma once
#include "TypeReferenceNode.h"
#include "TypedIdentifierNode.h"

class ParameterNode : public TypedIdentifierNode {
    bool is_reference_;

  public:
    ParameterNode(const FilePos& pos,
                  std::string name,
                  const TypeReferenceNode* type,
                  bool is_reference);

    ~ParameterNode() override;

    bool isIsReference() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
