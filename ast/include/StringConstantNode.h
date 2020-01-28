#pragma once
#include "ExpressionNode.h"
#include <string>

class StringConstantNode : public ConstantNode {

    std::string value_;

  public:
    StringConstantNode(const FilePos& pos, std::string value, TypeNode* type);
    ~StringConstantNode() override = default;

    const std::string& getValue() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};