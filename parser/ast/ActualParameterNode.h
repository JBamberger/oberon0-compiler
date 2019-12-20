#pragma once
#include "ExpressionNode.h"
#include "Node.h"

class ActualParameterNode : public Node {
    std::unique_ptr<ExpressionNode> param_;
    std::unique_ptr<ActualParameterNode> next_;

  public:
    explicit ActualParameterNode(std::unique_ptr<ExpressionNode> param);
    ~ActualParameterNode();
    const std::unique_ptr<ExpressionNode>& getParam() const;
    const std::unique_ptr<ActualParameterNode>& getNext() const;

    void visit(NodeVisitor* visitor) const override;

    void setNext(ActualParameterNode* next);

    void print(std::ostream& stream) const override;
};
