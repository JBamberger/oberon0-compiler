#pragma once
#include "Node.h"

class ExpressionNode : public Node {
  public:
    ExpressionNode(NodeType nodeType, const FilePos& pos) : Node(nodeType, pos) {}
};

class ConstantNode : public ExpressionNode {
  public:
    ConstantNode(NodeType nodeType, const FilePos& pos) : ExpressionNode(nodeType, pos) {}
};