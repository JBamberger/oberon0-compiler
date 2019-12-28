#pragma once
#include "Node.h"
#include "TypeNode.h"

class ExpressionNode : public Node {
    std::string type_;

  public:
    ExpressionNode(const NodeType node_type, const FilePos& pos, std::string type)
        : Node(node_type, pos), type_(std::move(type))
    {
    }

    std::string getType() const;

    void setType(const std::string& type_node);
};

class ConstantNode : public ExpressionNode {
  public:
    ConstantNode(const NodeType node_type, const FilePos& pos, std::string type)
        : ExpressionNode(node_type, pos, std::move(type))
    {
    }
};

class AssignableExpressionNode : public ExpressionNode {
  public:
    AssignableExpressionNode(const NodeType node_type, const FilePos& pos, std::string type)
        : ExpressionNode(node_type, pos, std::move(type))
    {
    }
};

inline std::string ExpressionNode::getType() const { return type_; }

inline void ExpressionNode::setType(const std::string& type_node) { type_ = type_node; }