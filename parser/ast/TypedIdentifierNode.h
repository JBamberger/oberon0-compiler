#pragma once
#include "Node.h"
#include <cassert>
#include "TypeNode.h"

class TypedIdentifierNode : public Node {
  protected:
    std::string name_;
    std::shared_ptr<TypeNode> type_;

  public:
    TypedIdentifierNode(const NodeType nodeType,
                        const FilePos& pos,
                        std::string name,
                        std::shared_ptr<TypeNode> type)
        : Node(nodeType, pos), name_(std::move(name)), type_(std::move(type))
    {
        assert(type_ != nullptr);
    }

    const std::string& getName() const { return name_; }
    std::shared_ptr<TypeNode> getType() const { return type_; }
};
