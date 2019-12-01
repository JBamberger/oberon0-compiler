#pragma once
#include "ExpressionNode.h"
#include <cassert>

class ChainedReferenceNode : public ExpressionNode {
  protected:
    std::unique_ptr<const ChainedReferenceNode> next_;

  public:
    explicit ChainedReferenceNode(const FilePos& pos)
        : ExpressionNode(NodeType::variable_reference, pos)
    {
    }

    void setNext(const ChainedReferenceNode* next)
    {
        next_ = std::unique_ptr<const ChainedReferenceNode>(next);
    }
};
