#pragma once
#include "Node.h"
#include "TypeReferenceNode.h"

template <NodeType T>
class TypedIdentifierNode : public Node {
    bool isReference = false;
    std::string name_;
    std::unique_ptr<const TypeReferenceNode> type_;

  public:
    TypedIdentifierNode(const FilePos& pos, std::string name, const TypeReferenceNode* type)
        : Node(T, pos), name_(std::move(name)), type_(type)
    {
        assert(type_ != nullptr);
    }

    ~TypedIdentifierNode() override = default;

    void makeReference() { isReference = true; }

    void print(std::ostream& stream) const override
    {
        stream << (isReference ? "FieldNodeRef(" : "FieldNode(") << name_ << ", " << *type_ << ")";
    }
};

using FieldNode = TypedIdentifierNode<NodeType::field>;
using VariableNode = TypedIdentifierNode<NodeType::field>;
using ParameterNode = TypedIdentifierNode<NodeType::parameter>;
