#pragma once
#include "ExpressionNode.h"
#include <cassert>

class SelectorNode : public ExpressionNode {
  protected:
    std::unique_ptr<SelectorNode> next_;

  public:
    explicit SelectorNode(const FilePos& pos) : ExpressionNode(NodeType::selector, pos) {}

    void setNext(std::unique_ptr<SelectorNode> next) { next_ = std::move(next); }

    const std::unique_ptr<SelectorNode>& getNext() const { return next_; }
};
