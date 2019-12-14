#pragma once
#include "ExpressionNode.h"
#include <cassert>

class SelectorNode : public ExpressionNode {
  protected:
    std::unique_ptr<const SelectorNode> next_;

  public:
    explicit SelectorNode(const FilePos& pos) : ExpressionNode(NodeType::selector, pos) {}

    void setNext(const SelectorNode* next) { next_ = std::unique_ptr<const SelectorNode>(next); }

    const std::unique_ptr<const SelectorNode>& getNext() const { return next_; }
};
