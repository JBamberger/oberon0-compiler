#pragma once
#include "ChainedReferenceNode.h"

class ArrayReferenceNode : public ChainedReferenceNode {

    std::unique_ptr<const ExpressionNode> index_;

  public:
    ArrayReferenceNode(const FilePos& pos, const ExpressionNode* index);
    ~ArrayReferenceNode() override;

    void print(std::ostream& stream) const override;
};