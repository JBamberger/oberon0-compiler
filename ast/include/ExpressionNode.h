#pragma once
#include "Node.h"
#include "TypeNode.h"

class ExpressionNode : public Node {
    TypeNode* type_;

  public:
    ExpressionNode(const FilePos& pos, TypeNode* type) : Node(pos), type_(type)
    {
    }

    TypeNode* getType() const
    {
        return type_;
    }

    void setType(TypeNode* type_node)
    {
        type_ = type_node;
    }
};

class ConstantNode : public ExpressionNode {
  public:
    ConstantNode(const FilePos& pos, TypeNode* type) : ExpressionNode(pos, type)
    {
    }
};

class AssignableExpressionNode : public ExpressionNode {
  public:
    AssignableExpressionNode(const FilePos& pos, TypeNode* type) : ExpressionNode(pos, type)
    {
    }
};
