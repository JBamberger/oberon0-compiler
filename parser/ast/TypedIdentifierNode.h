#pragma once
#include "Node.h"
#include "TypeNode.h"
#include <cassert>

class TypedIdentifierNode : public Node {
  protected:
    std::string name_;
    std::string type_;

  public:
    TypedIdentifierNode(const NodeType node_type,
                        const FilePos& pos,
                        std::string name,
                        std::string type)
        : Node(node_type, pos), name_(std::move(name)), type_(std::move(type))
    {
    }

    std::string getName() const { return name_; }
    std::string getType() const { return type_; }
};
