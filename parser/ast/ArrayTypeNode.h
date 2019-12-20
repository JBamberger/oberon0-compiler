#pragma once
#include "ExpressionNode.h"
#include "TypeNode.h"

class ArrayTypeNode : public TypeNode {
    std::unique_ptr<ExpressionNode> value_;
    std::unique_ptr<TypeNode> type_;

  public:
    ArrayTypeNode(const FilePos& pos,
                  std::unique_ptr<ExpressionNode> value,
                  std::unique_ptr<TypeNode> type);
    ~ArrayTypeNode() override;
    const std::unique_ptr<ExpressionNode>& getValue() const;
    const std::unique_ptr<TypeNode>& getType() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
