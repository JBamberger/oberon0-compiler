#pragma once
#include "Node.h"
#include "TypeNode.h"

class ExpressionNode : public Node {
    TypeNode* type_;

  public:
    ExpressionNode(const NodeType node_type, const FilePos& pos, TypeNode* type)
        : Node(node_type, pos), type_(type)
    {
    }

    TypeNode* getType() const { return type_; }

    void setType(TypeNode* type_node) { type_ = type_node; }
};

class ConstantNode : public ExpressionNode {
  public:
    ConstantNode(const NodeType node_type, const FilePos& pos, TypeNode* type)
        : ExpressionNode(node_type, pos, type)
    {
    }
};

class AssignableExpressionNode : public ExpressionNode {
  public:
    AssignableExpressionNode(const NodeType node_type, const FilePos& pos, TypeNode* type)
        : ExpressionNode(node_type, pos, type)
    {
    }
};
