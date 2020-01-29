#pragma once
#include "Node.h"
#include "TypeNode.h"
#include <cassert>

class TypedIdentifierNode : public Node {
  protected:
    std::string name_;
    TypeNode* type_;

  public:
    TypedIdentifierNode(const NodeType node_type,
                        const FilePos& pos,
                        std::string name,
                        TypeNode* type)
        : Node(node_type, pos), name_(std::move(name)), type_(type)
    {
    }

    ~TypedIdentifierNode() override = default;
    std::string getName() const
    {
        return name_;
    }
    TypeNode* getType() const
    {
        return type_;
    }
};
