#pragma once
#include "ExpressionNode.h"

class ArrayReferenceNode : public AssignableExpressionNode {

    std::unique_ptr<AssignableExpressionNode> array_ref_;
    std::unique_ptr<ExpressionNode> index_;

  public:
    ArrayReferenceNode(const FilePos& pos,
                       std::unique_ptr<ExpressionNode> index,
                       std::shared_ptr<TypeNode> type,
                       std::unique_ptr<AssignableExpressionNode> array_ref);

    ~ArrayReferenceNode() override;

    const std::unique_ptr<AssignableExpressionNode>& getArrayRef() const;
    const std::unique_ptr<ExpressionNode>& getIndex() const;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
