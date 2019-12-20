#pragma once
#include "Node.h"
#include "TypeReferenceNode.h"
#include <cassert>

class TypedIdentifierNode : public Node {

  protected:
    std::string name_;
    std::unique_ptr<const TypeReferenceNode> type_;

  public:
    TypedIdentifierNode(const NodeType nodeType,
                        const FilePos& pos,
                        std::string name,
                        const TypeReferenceNode* type)
        : Node(nodeType, pos), name_(std::move(name)), type_(type)
    {
        assert(type_ != nullptr);
    }

    const std::string& getName() const { return name_; }
    const std::unique_ptr<const TypeReferenceNode>& getType() const { return type_; }
};