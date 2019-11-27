#pragma once
#include "ExpressionNode.h"
#include "Node.h"

class ActualParameterNode : public Node {
    std::unique_ptr<const ExpressionNode> param_;
    std::unique_ptr<const ActualParameterNode> next_;

  public:
    ActualParameterNode(const ExpressionNode* param);
    ~ActualParameterNode();

    void setNext(const ActualParameterNode* next);
    void print(std::ostream& stream) const override;
};
