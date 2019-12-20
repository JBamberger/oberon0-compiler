#pragma once
#include "ExpressionNode.h"
#include "Node.h"

class ActualParameterNode : public Node {
    std::unique_ptr<ExpressionNode> param_;

  public:
    explicit ActualParameterNode(std::unique_ptr<ExpressionNode> param);

    ~ActualParameterNode();

    const std::unique_ptr<ExpressionNode>& getParam() const;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
