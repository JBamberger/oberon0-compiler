#pragma once
#include "ExpressionNode.h"
#include "Node.h"

class ActualParameterNode : public Node {
    std::unique_ptr<const ExpressionNode> param_;
    std::unique_ptr<const ActualParameterNode> next_;

  public:
    explicit ActualParameterNode(const ExpressionNode* param);
    ~ActualParameterNode();
    const std::unique_ptr<const ExpressionNode>& getParam() const;
    const std::unique_ptr<const ActualParameterNode>& getNext() const;

    void visit(NodeVisitor* visitor) const override;

    void setNext(const ActualParameterNode* next);

    void print(std::ostream& stream) const override;
};
