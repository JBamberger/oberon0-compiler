#pragma once
#include "SelectorNode.h"

class ArrayReferenceNode : public SelectorNode {

    std::unique_ptr<const ExpressionNode> index_;

  public:
    ArrayReferenceNode(const FilePos& pos, const ExpressionNode* index);
    ~ArrayReferenceNode() override;

    void print(std::ostream& stream) const override;
};
