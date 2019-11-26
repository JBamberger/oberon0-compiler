#pragma once
#include "ExpressionNode.h"

class VariableReferenceNode : public ExpressionNode {
  protected:
    std::unique_ptr<const VariableReferenceNode> next_;

  public:
    VariableReferenceNode(const FilePos& pos, const VariableReferenceNode* next)
        : ExpressionNode(NodeType::variable_reference, pos), next_(next)
    {
    }
    void setNext(const VariableReferenceNode* next)
    {
        next_ = std::unique_ptr<const VariableReferenceNode>(next);
    }
};