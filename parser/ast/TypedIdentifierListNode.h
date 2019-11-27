#pragma once
#include "IdentifierListNode.h"
#include "Node.h"
#include "TypeNode.h"
#include "TypedIdentifierNode.h"
#include <vector>

template <NodeType T>
class TypedIdentifierListNode : public Node {
    std::shared_ptr<std::vector<std::unique_ptr<const TypedIdentifierNode<T>>>> pairs_;
    std::shared_ptr<const TypeNode> type_;

  public:
    TypedIdentifierListNode(const FilePos& pos,
                            const IdentifierListNode* names,
                            const TypeNode* type,
                            const bool isReference)
        : Node(NodeType::typed_id_list, pos),
          pairs_(std::make_shared<std::vector<std::unique_ptr<const TypedIdentifierNode<T>>>>()),
          type_(type)
    {
        for (const auto& name : names->getNames()) {
            const auto fieldType = new TypeReferenceNode(type->getFilePos(), type_);
            auto field = new TypedIdentifierNode<T>(fieldType->getFilePos(), name, fieldType);
            if (isReference) {
                field->makeReference();
            }
            pairs_->emplace_back(field);
        }
    }

    TypedIdentifierListNode(const FilePos& pos,
                            const IdentifierListNode* names,
                            const TypeNode* type)
        : TypedIdentifierListNode(pos, names, type, false)
    {
    }
    ~TypedIdentifierListNode() override = default;

    std::shared_ptr<const TypeNode> getType() const { return type_; }
    std::shared_ptr<std::vector<std::unique_ptr<const TypedIdentifierNode<T>>>> getPairs() const
    {
        return pairs_;
    }
    void print(std::ostream& stream) const override
    {
        stream << "FieldListNode(";
        for (const auto& p : *pairs_) { stream << *p << " "; }
        stream << ", " << *type_ << ")";
    }
};

using FieldListNode = TypedIdentifierListNode<NodeType::field>;
using VariableListNode = TypedIdentifierListNode<NodeType::variable>;
using ParameterListNode = TypedIdentifierListNode<NodeType::parameter>;
