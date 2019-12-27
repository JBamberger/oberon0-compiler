#pragma once
#include "Node.h"
#include "TypeNode.h"

class ExpressionNode : public Node {
    std::shared_ptr<TypeNode> type_;

  public:
    ExpressionNode(const NodeType node_type, const FilePos& pos, std::shared_ptr<TypeNode> type)
        : Node(node_type, pos), type_(std::move(type))
    {
    }

    std::shared_ptr<TypeNode> getType() const;

    void setType(const std::shared_ptr<TypeNode>& type_node);
};

class ConstantNode : public ExpressionNode {
  public:
    ConstantNode(const NodeType node_type, const FilePos& pos, std::shared_ptr<TypeNode> type)
        : ExpressionNode(node_type, pos, std::move(type))
    {
    }
};

class AssignableExpressionNode : public ExpressionNode {
  public:
    AssignableExpressionNode(const NodeType node_type,
                             const FilePos& pos,
                             std::shared_ptr<TypeNode> type)
        : ExpressionNode(node_type, pos, std::move(type))
    {
    }
};

inline std::shared_ptr<TypeNode> ExpressionNode::getType() const { return type_; }

inline void ExpressionNode::setType(const std::shared_ptr<TypeNode>& type_node)
{
    type_ = type_node;
}