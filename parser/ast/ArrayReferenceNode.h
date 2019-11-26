#pragma once
#include "VariableReferenceNode.h"

class ArrayReferenceNode : public VariableReferenceNode {

    std::unique_ptr<const ExpressionNode> index_;

  public:
    ArrayReferenceNode(const FilePos& pos,
                       const VariableReferenceNode* next,
                       const ExpressionNode* index);
    virtual ~ArrayReferenceNode() override;

    void print(std::ostream& stream) const override;
};