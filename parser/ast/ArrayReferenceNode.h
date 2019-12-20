#pragma once
#include "SelectorNode.h"

class ArrayReferenceNode : public SelectorNode {

    std::unique_ptr<ExpressionNode> index_;

  public:
    ArrayReferenceNode(const FilePos& pos, std::unique_ptr<ExpressionNode> index);

    ~ArrayReferenceNode() override;

    const std::unique_ptr<ExpressionNode>& getIndex() const;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
