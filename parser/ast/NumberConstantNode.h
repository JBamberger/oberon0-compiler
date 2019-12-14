#pragma once
#include "ExpressionNode.h"

class NumberConstantNode : public ExpressionNode {

    int value_;

  public:
    NumberConstantNode(const FilePos& pos, int value);
    ~NumberConstantNode() override;

    int getValue() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};