#pragma once
#include "ExpressionNode.h"
#include "TypeNode.h"

class ArrayTypeNode : public TypeNode {
    std::unique_ptr<const ExpressionNode> value_;
    std::unique_ptr<const TypeNode> type_;

  public:
    ArrayTypeNode(const FilePos& pos, const ExpressionNode* value, const TypeNode* type);
    ~ArrayTypeNode() override;

    void print(std::ostream& stream) const override;
};
