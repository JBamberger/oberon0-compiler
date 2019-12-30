#pragma once
#include "ExpressionNode.h"
#include "NodeVisitor.h"

class BooleanConstantNode : public ConstantNode {

    int value_;

  public:
    BooleanConstantNode(const FilePos& pos, const int value)
        : ConstantNode(NodeType::number_constant, pos, "BOOLEAN"), value_(value)
    {
    }
    ~BooleanConstantNode() override = default;

    int getValue() const { return value_; }

    void setValue(const int value) { this->value_ = value; }

    void visit(NodeVisitor* visitor) const override { visitor->visit(this); }

    void print(std::ostream& stream) const override
    {
        stream << "BooleanConstant(" << value_ << ")";
    }
};
