#pragma once
#include "ExpressionNode.h"

class NumberConstantNode : public ExpressionNode {

    int value_;

  public:
    NumberConstantNode(const FilePos& pos, int value);
    virtual ~NumberConstantNode() override;

    void print(std::ostream& stream) const override;
};