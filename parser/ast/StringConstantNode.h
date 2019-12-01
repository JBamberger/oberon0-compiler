#pragma once
#include "ExpressionNode.h"
#include <string>

class StringConstantNode : public ExpressionNode {

    std::string value_;

  public:
    StringConstantNode(const FilePos& pos, std::string value);
    ~StringConstantNode() override = default;

    void print(std::ostream& stream) const override;
};