#pragma once
#include "ExpressionNode.h"

class NumberConstantNode : public ExpressionNode {

    int value_;

  public:
    NumberConstantNode(const FilePos& pos, int value);
    ~NumberConstantNode() override;

    void print(std::ostream& stream) const override;
};